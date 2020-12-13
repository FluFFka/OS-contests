#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
    uint32_t n, s, w;
    if (scanf("%" SCNu32 "%" SCNu32 "%" SCNu32, &n, &s, &w) != 3) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    uint32_t i = 0;
    while (1) {
        putchar('|');
        printf("%*o|", w, i);
        printf("%*u|", w, i);
        if (i & (1U << (n - 1))) {
            int32_t out_curr = (-1) * (i & ~(1U << (n - 1)));
            printf("%*d|\n", w, out_curr);
        } else {
            printf("%*d|\n", w, i);
        }
        if (1LL * i + s >= 1LL << n) {
            break;
        }
        i += s;
    }
    return 0;
}
