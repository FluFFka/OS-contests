#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

enum
{
    KIB = 64 * 1024,
    MASK_CHG = 0x1FF,
    PAGE_SHIFT = 9,
    SHIFT = 8
};

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    uint16_t table_addr;
    if (sscanf(argv[2], "%" SCNx16, &table_addr) != 1) {
        fprintf(stderr, "Invalid argument\n");
        return 1;
    }
    size_t sys_page_size = sysconf(_SC_PAGESIZE);
    size_t map_size = KIB;
    if (map_size % sys_page_size != 0) {
        map_size += sys_page_size - map_size % sys_page_size;
    }
    unsigned char *memory = mmap(NULL, map_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (memory == MAP_FAILED) {
        fprintf(stderr, "Can't map memory\n");
        return 1;
    }
    uint16_t curr;
    while (scanf("%" SCNx16, &curr) == 1) {
        uint16_t page_num = *((uint16_t *) (memory + table_addr + 2 * (curr >> PAGE_SHIFT)));
        curr = page_num | (curr & MASK_CHG);
        printf("%u\n", *((uint16_t *) (memory + curr)));
    }
    munmap(memory, map_size);
    close(fd);
    return 0;
}
