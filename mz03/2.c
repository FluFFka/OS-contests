#include <stdio.h>
#include <inttypes.h>

int
main(int argc, char *argv[])
{
    int64_t pos = 0, neg = 0;
    for (int i = 1; i < argc; ++i) {
        int curr;
        sscanf(argv[i], "%d", &curr);
        if (curr > 0) {
            pos += curr;
        } else {
            neg += curr;
        }
    }
    printf("%" PRId64 "\n%" PRId64 "\n", pos, neg);
    return 0;
}
