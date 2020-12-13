#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    for (int i = 0; i < n - 1; ++i) {
        printf("%d ", i + 1);
        fflush(stdout);
        if (fork()) {
            while (wait(NULL) != -1);
            return 0;
        }
    }
    printf("%d\n", n);
    fflush(stdout);
    return 0;
}
