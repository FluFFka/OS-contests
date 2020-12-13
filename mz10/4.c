#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

void safeclose(int retsc, int *pids, int size) {
    if (retsc == -1) {
        for (int i = 0; i < size; ++i) {
            kill(pids[i], SIGKILL);
        }
        while (wait(NULL) != -1 || errno != ECHILD);
        _exit(1);
    }
}

void safe(int retsc) {
    if (retsc == -1) {
        _exit(1);
    }
}

int
main(int argc, char *argv[])
{
    int prev[2], next[2];
    safe(pipe(prev) == -1);
    safe(close(prev[1]));
    safe(dup2(0, prev[0]));
    int *children = calloc(argc - 1, sizeof(children[0]));
    if (!children) {
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        safeclose(pipe(next), children, i - 1);
        int pid;
        safeclose((pid = fork()), children, i - 1);
        children[i - 1] = pid;
        if (!pid) {
            close(next[0]);
            if (i < argc - 1) {
                dup2(next[1], 1);
            }
            close(next[1]);
            dup2(prev[0], 0);
            close(prev[0]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        safeclose(close(next[1]), children, i);
        safeclose(close(prev[0]), children, i);
        safeclose(dup2(next[0], prev[0]), children, i);
        safeclose(close(next[0]), children, i);
    }
    safeclose(close(prev[0]), children, argc - 1);
    int retsc = 0, end = 0;
    do {
        retsc = wait(NULL);
        if (retsc == -1) {
            if (errno != ECHILD) {
                end = 1;
            }
        }
    } while (retsc != -1 || errno != ECHILD);
    free(children);
    return end;
}
