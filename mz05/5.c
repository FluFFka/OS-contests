#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    OFFSET = 8,
    BUF_SIZE = 2
};

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    uint16_t ans = 0;
    char flag = 0;
    while (1) {
        uint16_t num = 0;
        unsigned char buf[BUF_SIZE];
        if (read(fd, buf, sizeof(buf)) != sizeof(buf)) {
            break;
        }
        num = ((num | buf[0]) << OFFSET) | buf[1];
        if (num % 2 == 0) {
            if (!flag) {
                flag = 1;
                ans = num;
            } else if (ans > num) {
                ans = num;
            }
        }
    }
    if (flag) {
        printf("%" PRIu16 "\n", ans);
    }
    return 0;
}
