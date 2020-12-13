#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>

enum
{
    BUF_SIZE = 4096,
    NUMBER_OF_DIGITS = 10    
};

int
main(void)
{
    char buf[BUF_SIZE];
    uint64_t res[NUMBER_OF_DIGITS];
    if (!fgets(buf, sizeof(buf), stdin)) {
        buf[0] = '\0';
    }
    for (int i = 0; i < sizeof(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';
        }
    }
    int fd = open(buf, O_RDONLY);
    int curr_size = 0;
    if (fd != -1) {
        while ((curr_size = read(fd, buf, sizeof(buf))) > 0) {
            for (int i = 0; i < curr_size; ++i) {
                if (buf[i] >= '0' && buf[i] <= '9') {
                    res[buf[i] - '0']++;
                }
            }
        } 
        close(fd);
    }
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) { 
        printf("%d %" PRIu64 "\n", i, res[i]);
    }
    return 0;
}
