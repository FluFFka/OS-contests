#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned long long hash_function(unsigned long long);

enum { MODE = 0660 };

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    unsigned long long h0;
    int count;
    sscanf(argv[2], "%llx", &h0);
    sscanf(argv[3], "%d", &count);

    int fd = open(argv[1], O_CREAT | O_WRONLY, MODE);
    if (fd == -1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }   
    off_t file_end = lseek(fd, 0, SEEK_END);
    if (file_end == -1) {
        fprintf(stderr, "Can't move in file\n");
        return 1;
    }
    for (int i = count - 1; i >= 0; --i) {
        if (lseek(fd, file_end + i * sizeof(h0), SEEK_SET) == -1) {
            fprintf(stderr, "Can't move in file\n");
            return 1;
        }
        if (write(fd, &h0, sizeof(h0)) != sizeof(h0)) {
            fprintf(stderr, "Invalid write\n");
            return 1;
        }
        h0 = hash_function(h0);
    }
    close(fd);
    return 0;
}
