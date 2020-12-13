#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int fl = 0;

void
handler(int signum)
{
    if (signum == SIGINT) {
        fl = 0;
    } else {
        fl = 1;
    }
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
}

int
main(void)
{
    int value = 0, num;
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
    printf("%d\n", getpid());
    fflush(stdout);
    while (scanf("%d", &num) == 1) {
        if (!fl) {
            __builtin_sadd_overflow(value, num, &value);
        } else {
            __builtin_smul_overflow(value, num, &value);
        }
        printf("%d\n", value);
        fflush(stdout);
    }
    return 0;
}
