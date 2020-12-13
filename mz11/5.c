#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

enum
{
    MAX_CNT = 4
};

volatile int prime = 0;
volatile int cnt = 0;

void
handler(int signum)
{
    if (signum == SIGINT) {
        cnt++;
        if (cnt >= MAX_CNT) {
            _exit(0);
        }
        printf("%d\n", prime);
        fflush(stdout);
    }
    if (signum == SIGTERM) {
        _exit(0);
    }
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
}

void
is_prime(int num)
{
    for (int i = 2; 1LL * i * i <= num; ++i) {
        if (num % i == 0) {
            return;
        }
    }
    prime = num;
}

int
main(void)
{
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    int low, high;
    scanf("%d%d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    if (low < 2) {
        low = 2;
    }
    for (int i = low; i < high; ++i) {
        is_prime(i);
    }
    printf("-1\n");
    fflush(stdout);
    return 0;
}
