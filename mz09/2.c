#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int
mysys(const char *str)
{
    int pid = fork();
    if (pid == -1) {
        return -1;
    } else if (pid == 0) {
        if (execl("/bin/sh", "sh", "-c", str, NULL) == -1) {
            _exit(127);
        }
    }
    int st;
    waitpid(pid, &st, 0);
    if (WIFSIGNALED(st)) {
        return WTERMSIG(st) + 128;
    }
    return WEXITSTATUS(st);
}
