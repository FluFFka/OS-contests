#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

enum
{
    STR_SIZE = 8,
    STR_NUM = 3
};

int
main(void)
{
    for (int i = 0; i < STR_NUM; ++i) {
        if (!fork()) {
            char buf[STR_SIZE];
            if (read(0, buf, STR_SIZE) != STR_SIZE) {
                fprintf(stderr, "Can't read number\n");
                return 1;
            }
            int num;
            if (sscanf(buf, "%d", &num) != 1) {
                fprintf(stderr, "Can't read number\n");
                return 1;
            }
            printf("%d %d\n", i + 1, num * num);
            return 0;
        }
    }
    while (wait(NULL) != -1);
    return 0;
}
