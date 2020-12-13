#include <stdio.h>

int
main(int argc, char *argv[])
{
    char pattern[] = "rwxrwxrwx"; //reverse
    for (int i = 1; i < argc; ++i) {
        unsigned int curr;
        sscanf(argv[i], "%o", &curr);
        for (int check = 0; check < sizeof(pattern) - 1; ++check) {
            if (curr & (1 << (sizeof(pattern) - 1 - check - 1))) {
                putchar(pattern[check]);
            } else {
                putchar('-');
            }
        }
        putchar('\n');
    }
}
