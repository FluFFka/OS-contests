#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

void
trav(char *stpath, char *path, int deep, int Z)
{
    if (deep >= 4) {
        return;
    }
    DIR *dir = opendir(path);
    if (!dir) {
       return;
    }
    struct dirent *dir_info;
    while ((dir_info = readdir(dir))) {
        char PATH[PATH_MAX];
        snprintf(PATH, sizeof(PATH), "%s/%s", path, dir_info->d_name);
        if (!strcmp(".", dir_info->d_name) || !strcmp("..", dir_info->d_name)) {
            continue;
        }
        struct stat buf;
        if (lstat(PATH, &buf)) {
            continue;
        }
        if (S_ISREG(buf.st_mode)) {
            if (buf.st_size <= Z) {
                if (!access(PATH, R_OK)) {
                    printf("%s\n", PATH);
                }
            }
        } else if (S_ISDIR(buf.st_mode)) {
            trav(stpath, PATH, deep + 1, Z);
        }
    }
    closedir(dir);
    return;
}

int
main(int argc, char *argv[])
{
    int sz;
    sscanf(argv[2], "%d", &sz);
    trav(argv[1], argv[1], 1, sz);

    return 0;
}
