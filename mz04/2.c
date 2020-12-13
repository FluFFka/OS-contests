#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

enum
{
    BUF_SIZE = 5
};


int
main(int argc, char *argv[])
{
    int buf[BUF_SIZE];
    int flag = 0, first = 0, second = 0;
    for (int i = 1; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            continue;
        }
        int num;
        while (read(fd, buf, sizeof(buf)) == sizeof(buf)) {
            num = buf[BUF_SIZE - 1];
            if (flag == 0) {
                flag = 1;
                first = num;
            } else if (flag == 1 && first != num) {
                flag = 2;
                if (first > num) {
                    second = num;
                } else if (first != num) {
                    second = first;
                    first = num;
                }
            } else if (flag == 2) {
                if (first > num) {
                    if (second < num) {
                        second = num;
                    }
                } else if (first != num) {
                    second = first;
                    first = num;
                }
            }
        }
        close(fd);
    }
    if (flag == 2) {
        if (second >= 0) {
            printf("%d.%02d\n", second / 100, second % 100);
        } else {
            printf("-%lld.%02lld\n", (-1LL) * second / 100, (-1LL) * second % 100);
        }
    }
    return 0;
}
