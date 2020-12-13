#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    MODE = 0600,
    BUF_SIZE = 4,
    MASK_LAST = 0xFF,
    MASK_MID1 = 0xF00,
    MASK_MID2 = 0xFF000,
    MASK_FIRST = 0xF00000,
    OFF_MID1 = 8,
    OFF_MID2 = 12,
    OFF_FIRST = 20
};

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_TRUNC | O_CREAT | O_WRONLY, MODE);
    unsigned int num;
    unsigned char buf[BUF_SIZE];
    while (scanf("%u", &num) > 0) {
        buf[0] = (num & MASK_FIRST) >> OFF_FIRST;
        buf[1] = (num & MASK_MID2) >> OFF_MID2;
        buf[2] = (num & MASK_MID1) >> OFF_MID1;
        buf[3] = (num & MASK_LAST);
        write(fd, buf, sizeof(buf));
    }
    close(fd); 
    return 0;
}
