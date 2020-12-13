#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int
comp(const void *s1, const void *s2)
{
    return strcasecmp(*(char **)s1,  *(char **)s2);
}

void
trav(char *path, char *short_path, int root)
{
    DIR *dir = opendir(path);
    if (!dir) {
        return;
    }
    if (!root) {
        printf("cd %s\n", short_path);
    }
    int sz = 1, curr = 0;
    char **all_dirs;
    all_dirs = calloc(sz, sizeof(char *));
    struct dirent *dir_info;
    while ((dir_info = readdir(dir))) {
        if (curr >= sz) {
            sz *= 2;
            all_dirs = realloc(all_dirs, sz * sizeof(char *));
        }
        all_dirs[curr] = calloc(PATH_MAX, sizeof(char));
        sprintf(all_dirs[curr], "%s", dir_info->d_name);
        curr++;
    }
    qsort(all_dirs, curr, sizeof(char *), comp);
    closedir(dir);
    for (int i = 0; i < curr; ++i) {
        if (all_dirs[i] && strcmp(all_dirs[i], ".") && strcmp(all_dirs[i], "..")) {
            char next_path[PATH_MAX];
            if (strlen(path) + strlen(all_dirs[i]) > PATH_MAX - 1) {
                continue;
            }
            sprintf(next_path, "%s/%s", path, all_dirs[i]);
            struct stat buf;
            if (!lstat(next_path, &buf)) {
                if (S_ISDIR(buf.st_mode) && !S_ISLNK(buf.st_mode)) {
                    trav(next_path, all_dirs[i], 0);
                }
            }
        }
        free(all_dirs[i]);
    }
    free(all_dirs);
    if (!root) {
        printf("cd ..\n");
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    trav(argv[1], argv[1], 1);
    return 0;
}
