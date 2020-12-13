#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int
main(void)
{
    if (fork() == 0) {
        if (fork() == 0) {
            printf("3 ");
            fflush(stdout);
            _exit(0);
        }
        while (wait(NULL) != -1);
        printf("2 ");
        fflush(stdout);
        _exit(0);
    }
    while (wait(NULL) != -1);
    printf("1\n");
    return 0;
}
