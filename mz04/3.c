#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

enum
{
    STRUCT_SIZE = 12,
    BUF_SIZE = 3
};

int
dfs(int fd, int idx)
{
    off_t offset = idx * STRUCT_SIZE; 
    if (lseek(fd, offset, SEEK_SET) == -1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    int32_t buf[BUF_SIZE];
    int32_t key, left_idx, right_idx;
    if (read(fd, buf, sizeof(buf)) != sizeof(buf)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    key = buf[0];
    left_idx = buf[1];
    right_idx = buf[2];
    if (right_idx) {
        int dfs_ret = dfs(fd, right_idx);
        if (dfs_ret) {
            return dfs_ret;
        }
    }
    printf("%" PRId32 " ", key);
    if (left_idx) {
        int dfs_ret = dfs(fd, left_idx);
        if (dfs_ret) {
            return dfs_ret;
        }
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    int dfs_ret = dfs(fd, 0);
    if (dfs_ret) {
        return dfs_ret;
    }
    putchar('\n');
    close(fd);
    return 0;
}
