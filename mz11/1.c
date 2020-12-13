#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

enum
{
    MAX_CNT = 6
};

void
handler(int signum)
{
    static int cnt = 0;
    cnt++;
    if (cnt < MAX_CNT) {
        printf("%d\n", cnt - 1);
        fflush(stdout);
        signal(SIGHUP, handler);
    } else {
        _exit(0);
    }

}

int
main(int argc, char *argv[])
{
    signal(SIGHUP, handler);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 0;
}
