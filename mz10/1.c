#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum
{
    ERA_START = 1900
};

int
main(void)
{
    int p[2];
    if (pipe(p) == -1) {
        fprintf(stderr, "Can't create pipe\n");
        return 1;
    }
    if (!fork()) {
        if (!fork()) {
            if (!fork()) {
                close(p[0]);
                time_t curr = time(NULL);
                write(p[1], &curr, sizeof(curr));
                write(p[1], &curr, sizeof(curr));
                write(p[1], &curr, sizeof(curr));
                close(p[1]);
                _exit(0);
            }
            wait(NULL);
            close(p[1]);
            time_t curr;
            read(p[0], &curr, sizeof(curr));
            close(p[0]);
            struct tm *ctime = localtime(&curr);
            int day = ctime->tm_mday;
            printf("D:%02d\n", day);
            fflush(stdout);



            _exit(0);
        }
        wait(NULL);
        close(p[1]);
        time_t curr;
        read(p[0], &curr, sizeof(curr));
        close(p[0]);
        struct tm *ctime = localtime(&curr);
        int mon = ctime->tm_mon + 1;
        printf("M:%02d\n", mon);
        fflush(stdout);
        _exit(0);
    }
    wait(NULL);
    close(p[1]);
    time_t curr;
    read(p[0], &curr, sizeof(curr));
    close(p[0]);
    struct tm *ctime = localtime(&curr);
    int year = ctime->tm_year + ERA_START;
    printf("Y:%04d\n", year);
    fflush(stdout);

    return 0;
}
