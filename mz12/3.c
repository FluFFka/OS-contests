#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

volatile int counter = 0;

void
handler1(int signum)
{
    signal(SIGUSR1, handler1);
    counter += 5;
    printf("%d %d\n", signum, counter);
    fflush(stdout);
    if (counter < 0) {
        _exit(0);
    }
}

void
handler2(int signum)
{
    signal(SIGUSR2, handler2);
    counter -= 4;
    printf("%d %d\n", signum, counter);
    fflush(stdout);
    if (counter < 0) {
        _exit(0);
    }
}

int
main(void)
{
    signal(SIGUSR1, handler1);
    signal(SIGUSR2, handler2);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 0;
}
