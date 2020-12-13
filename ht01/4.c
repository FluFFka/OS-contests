#include <stdio.h>
#include <dlfcn.h>
#include <inttypes.h>
#include <string.h>

enum
{
    ARG_SIZE = 64,
    ARG_SHIFT = 4
};

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    char *err;
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if ((err = dlerror())) {
        printf("%s\n", err);
        return 1;
    }
    void *sym = dlsym(handle, argv[2]);
    if ((err = dlerror())) {
        printf("%s\n", err);
        return 1;
    }
    int off = 0;
    unsigned char buf[ARG_SIZE] = {0};
    for (int i = 0; i < argc - ARG_SHIFT; ++i) {
        if (argv[3][i + 1] == 's') {
            memcpy(buf + off, argv + i + ARG_SHIFT, sizeof(char *));
            off += sizeof(char *);
        } else if (argv[3][i + 1] == 'i') {
            int num;
            sscanf(argv[i + ARG_SHIFT], "%d", &num);
            memcpy(buf + off, &num, sizeof(num));
            off += sizeof(num);
        } else {
            double num;
            sscanf(argv[i + ARG_SHIFT], "%lf", &num);
            memcpy(buf + off, &num, sizeof(num));
            off += sizeof(num);
        }
    }
    if (argv[ARG_SHIFT - 1][0] == 'v') {
        ((void (*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t))(intptr_t)sym)(*((uint64_t *)(buf)), *((uint64_t *)(buf + 8)),
                *((uint64_t *)(buf + 16)),*((uint64_t *)(buf + 24)),*((uint64_t *)(buf + 32)),
                *((uint64_t *)(buf + 40)),*((uint64_t *)(buf + 48)),*((uint64_t *)(buf + 56)));
    } else if (argv[ARG_SHIFT - 1][0] == 's') {
        char *res;
        res = ((char *(*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t))(intptr_t)sym)(*((uint64_t *)(buf)), *((uint64_t *)(buf + 8)),
                *((uint64_t *)(buf + 16)),*((uint64_t *)(buf + 24)),*((uint64_t *)(buf + 32)),
                *((uint64_t *)(buf + 40)),*((uint64_t *)(buf + 48)),*((uint64_t *)(buf + 56)));
        printf("%s\n", res);
    } else if (argv[ARG_SHIFT - 1][0] == 'i') {
        int res;
        res = ((int (*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t))(intptr_t)sym)(*((uint64_t *)(buf)), *((uint64_t *)(buf + 8)),
                *((uint64_t *)(buf + 16)),*((uint64_t *)(buf + 24)),*((uint64_t *)(buf + 32)),
                *((uint64_t *)(buf + 40)),*((uint64_t *)(buf + 48)),*((uint64_t *)(buf + 56)));
        printf("%d\n", res);
    } else {
        double res;
        res = ((double (*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t))(intptr_t)sym)(*((uint64_t *)(buf)), *((uint64_t *)(buf + 8)),
                *((uint64_t *)(buf + 16)),*((uint64_t *)(buf + 24)),*((uint64_t *)(buf + 32)),
                *((uint64_t *)(buf + 40)),*((uint64_t *)(buf + 48)),*((uint64_t *)(buf + 56)));

        printf("%.10g\n", res);
    }
    dlclose(handle);
    return 0;
}
