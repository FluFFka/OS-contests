#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    long long curr_num, find_num;
    int curr_ind = -1, find_ind = -1;
    while (read(fd, &curr_num, sizeof(curr_num)) == sizeof(curr_num)) {
        curr_ind++;
        if (find_ind == -1) {
            find_ind = curr_ind;
            find_num = curr_num;
        } else if (curr_num < find_num) {
            find_ind = curr_ind;
            find_num = curr_num;
        }
    }
    if (find_ind != -1) {
        lseek(fd, find_ind * sizeof(find_num), SEEK_SET);
        if (find_num != LLONG_MIN) {
            find_num = -find_num;
        }
        if (write(fd, &find_num, sizeof(find_num)) != sizeof(find_num)) {
            fprintf(stderr, "Invalid write\n");
            return 1;
        }
    }
    close(fd);
    return 0;
}
