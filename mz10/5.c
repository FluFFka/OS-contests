#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

void
f(int fin, int fout, int n, int que)
{
    int num = 1;
    while (num < n) {
        read(fin, &num, sizeof(num));
        if (num >= n) {
            break;
        }
        printf("%d %d\n", que, num);
        fflush(stdout);
        num++;
        write(fout, &num, sizeof(num));
    }
    _exit(0);
}

int
main(int argc, char *argv[])
{
    int n;
    sscanf(argv[1], "%d", &n);
    
    int p1[2];
    pipe(p1);
    int p2[2];
    pipe(p2);

    int num = 1;
    write(p1[1], &num, sizeof(num));
    if (!fork()) {
        f(p1[0], p2[1], n, 1);
    } else if (!fork()) {
        f(p2[0], p1[1], n, 2);
    }
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    while (wait(NULL) != -1);
    printf("Done\n");
    return 0;
}
