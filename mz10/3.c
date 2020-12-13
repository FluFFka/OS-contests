#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (!fork()) {
        close(p[0]);
        int pid;
        if ((pid = fork()) == 0) {
            int fd = open(argv[4], O_RDONLY);
            if (fd == -1) {
                _exit(1);
            }
            dup2(fd, 0);
            close(fd);
            dup2(p[1], 1);
            close(p[1]);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        int st;
        waitpid(pid, &st, 0);
        if (WIFEXITED(st) && (WEXITSTATUS(st) == 0)) {
            dup2(p[1], 1);
            close(p[1]);
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
        _exit(0);
    }
    close(p[1]);
    if (!fork()) {
        int fd = open(argv[5], O_APPEND | O_CREAT | O_WRONLY, 0664);
        if (fd == -1) {
            _exit(1);
        }
        dup2(fd, 1);
        close(fd);
        dup2(p[0], 0);
        close(p[0]);
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }
    close(p[0]);
    while (wait(NULL) != -1);
    return 0;
}
