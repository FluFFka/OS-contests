#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int
min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (fd == -1) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    int rows, cols;
    if (sscanf(argv[2], "%d", &rows) != 1 || sscanf(argv[3], "%d", &cols) != 1) {
        fprintf(stderr, "Invalid argument\n");
        return 1;
    }
    int matrix_size = rows * cols;
    if (ftruncate(fd, matrix_size * sizeof(int))) {
        fprintf(stderr, "Can't fill file\n");
        return 1;
    }
    size_t sys_page_size = sysconf(_SC_PAGESIZE);
    size_t map_size = matrix_size * sizeof(int);
    if (map_size % sys_page_size != 0) {
        map_size += sys_page_size - map_size % sys_page_size;
    }
    int *memory = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        fprintf(stderr, "Can't map file\n");
        return 1;
    }
    memset(memory, 0, matrix_size * sizeof(int));
    int num = 1;
    for (int i = 0; i < (min(rows, cols) + 1) / 2; ++i) {
        for (int j = i; j < cols - i; ++j, ++num) {
            *(memory + i * cols + j) = num;
        }
        for (int j = i + 1; j < rows - i; ++j, ++num) {
            *(memory + j * cols + cols - i - 1) = num;
        }
        for (int j = cols - i - 2; j >= i && rows - i - 1 > i; --j, ++num) {
            *(memory + (rows - i - 1) * cols + j) = num;
        }
        for (int j = rows - i - 2; j > i && cols - i - 1 > i; --j, ++num) {
            *(memory + j * cols + i) = num;
        }
    }
    munmap(memory, map_size);
    close(fd);
    return 0;
}
