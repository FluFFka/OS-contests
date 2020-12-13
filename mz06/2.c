#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

enum { STR_SIZE = 4 };

int
main(int argc, char *argv[])
{
    DIR *dir = opendir(argv[1]);
    struct dirent *dir_info;
    int ans = 0;
    while ((dir_info = readdir(dir))) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", argv[1], dir_info->d_name);
        struct stat buf;
        if (!stat(path, &buf)) {
            if (S_ISREG(buf.st_mode) && !access(path, X_OK)
                    && !strcmp(".exe", dir_info->d_name + strlen(dir_info->d_name) - STR_SIZE)) {
                ans++;
            }
        }
    }
    closedir(dir);
    printf("%d\n", ans);
    return 0;
}
