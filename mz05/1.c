#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

enum { KIB = 1024 };

int
main(int argc, char *argv[])
{
    off_t size = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat buf;
        if (lstat(argv[i], &buf) != -1 && S_ISREG(buf.st_mode)
                && buf.st_nlink == 1 && buf.st_size % KIB == 0) {
            size += buf.st_size;
        }
    }
    printf("%llu\n", size);
    return 0;
}
