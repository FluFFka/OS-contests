#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(void)
{
    int fd[2];
    pipe(fd);
    if (!fork()) {
        close(fd[1]);
        if (!fork()) {
            long long sum = 0;
            int num;
            while (read(fd[0], &num, sizeof(num)) == sizeof(num)) {
                sum += num;
            }
            close(fd[0]);
            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        }
        close(fd[0]);
        wait(NULL);
        _exit(0);
    }
    close(fd[0]);
    int num;
    while (scanf("%d", &num) == 1) {
        write(fd[1], &num, sizeof(num));
    }
    close(fd[1]);
    wait(NULL);

    return 0;
}
