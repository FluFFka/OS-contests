#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        return 1;
    }
    int count = atoi(argv[1]);
    int modulo = atoi(argv[2]);
    int fd2to4[2];
    pipe(fd2to4);
    if (!fork()) {
        int fd1to2[2];
        pipe(fd1to2);
        if (!fork()) {
            close(fd2to4[0]);
            close(fd1to2[1]);
            dup2(fd1to2[0], 0);
            close(fd1to2[0]);
            dup2(fd2to4[1], 1);
            close(fd2to4[1]);
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
        close(fd2to4[0]);
        close(fd2to4[1]);
        close(fd1to2[0]);
        dup2(fd1to2[1], 1);
        close(fd1to2[1]);
        for (unsigned long long i = 1; i <= count; ++i) {
            printf("%d ", (int) ((i * i) % modulo));
            fflush(stdout);
        }
        close(1);
        wait(NULL);
        _exit(0);
    }
    if (!fork()) {
        if (!fork()) {
            close(fd2to4[1]);
            dup2(fd2to4[0], 0);
            close(fd2to4[0]);
            int num;
            while (scanf("%d", &num) == 1) {
                printf("%d\n", num);
                fflush(stdout);
            }
            close(0);
            _exit(0);
        }
        close(fd2to4[0]);
        close(fd2to4[1]);
        wait(NULL);
        _exit(0);
    }
    close(fd2to4[0]);
    close(fd2to4[1]);
    while (wait(NULL) != -1);
    printf("0\n");
    fflush(stdout);
    return 0;
}
