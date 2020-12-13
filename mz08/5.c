#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(void)
{
    int n;
    int fpid = fork();
    if (fpid == -1) {
        printf("-1\n");
        fflush(stdout);
        return 0;
    }
    if (fpid == 0) {
        while (scanf("%d", &n) == 1) {
            int pid = fork();
            if (pid == -1) {
                printf("-1\n");
                fflush(stdout);
                _exit(1);
            }
            if (pid != 0) {
                int status;
                wait(&status);
                if (status != 0) {
                    _exit(1);
                }
                printf("%d\n", n);
                fflush(stdout);
                _exit(0);
            }
        }
        _exit(0);
    }
    while (wait(NULL) != -1);
    return 0;
}
