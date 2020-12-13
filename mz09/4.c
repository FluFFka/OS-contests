#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

enum
{
    EXTRA_LEN = 15
};

int
main(int argc, char *argv[])
{
    int pid = getpid(), fd;
    char path[PATH_MAX] = {0};
    char *dir;
    if ((dir = getenv("XDG_RUNTIME_DIR"))) {
        sprintf(path, "%s/%d.py", dir, pid);
        fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    } else if ((dir = getenv("TMPDIR"))) {
        sprintf(path, "%s/%d.py", dir, pid);
        fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    } else {
        sprintf(path, "/tmp/%d.py", pid);
        fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    }
    FILE *file = fdopen(fd, "w");
    fprintf(file, "#!/usr/bin/python3\nprint(");
    for (int i = 1; i < argc; ++i) {
        fprintf(file, "%s", argv[i]);
        if (i != argc - 1) {
            fprintf(file, "*");
        } else {
            fprintf(file, ")");
        }
    }
    fclose(file);
    char com[2 * PATH_MAX + EXTRA_LEN];
    sprintf(com, "python3 %s && rm %s", path, path);
    execl("/bin/sh", "sh", "-c", com, NULL);
    return 0;
}
