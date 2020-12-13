#include <stdio.h>
#include <stdlib.h>

enum
{
    LOWER_OFF = 1 + '9' - '0' + 1,
    UPPER_OFF = 1 + '9' - '0' + 1 + 'z' - 'a' + 1,
    INVERT = 0x8,
    CHANGE_ZERO = 0x3B,
    RES_SPECIAL_1 = 0,
    RES_SPECIAL_2 = 63
};

int
main(void)
{
    int c;
    while ((c = getchar()) != EOF) {
        if (c >= '0' && c <= '9') {
            c = c - '0' + 1;
        } else if (c >= 'a' && c <= 'z') {
            c = c - 'a' + LOWER_OFF;
        } else if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + UPPER_OFF;
        } else {
            continue;
        }
        c = (c ^ INVERT) & CHANGE_ZERO;
        if (c == RES_SPECIAL_1) {
            putchar('@');
        } else if (c == RES_SPECIAL_2) {
            putchar('#');
        } else if (c >= 1 && c < LOWER_OFF) {
            putchar(c - 1 + '0');
        } else if (c >= LOWER_OFF && c < UPPER_OFF) {
            putchar(c - LOWER_OFF + 'a');
        } else {
            putchar(c - UPPER_OFF + 'A');
        }
    }
    putchar('\n');
    return 0;
}
