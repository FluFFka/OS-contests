#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int ex = 0;
    int N = atoi(argv[1]);
    for (int i = 2; i < 2 + N && i < argc; ++i) {
        FILE *in = fopen(argv[i], "r");
        char buf[PATH_MAX];
        fscanf(in, "%s", buf);
        if (!fork()) {
            execlp(buf, buf, NULL);
            _exit(1);
        }
    }
    int st;
    while (wait(&st) != -1) {
        if (WIFEXITED(st) && WEXITSTATUS(st) == 0) {
            ex++;
        }
    }
    for (int i = 2 + N; i < argc; ++i) {
        FILE *in = fopen(argv[i], "r");
        char buf[PATH_MAX];
        fscanf(in, "%s", buf);
        if (!fork()) {
            execlp(buf, buf, NULL);
            _exit(1);
        }
        while (wait(&st) != -1) {
            if (WIFEXITED(st) && WEXITSTATUS(st) == 0) {
                ex++;
            }
        }

    }
    printf("%d\n", ex);
    fflush(stdout);
    return 0;
}
