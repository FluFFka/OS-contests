#include <stdio.h>
#include <time.h>
#include <string.h>

enum
{
    NUM_YDAYS = 367,
    BAD_DAY = 3,
    THURSDAY = 4
};

int
main(void)
{
    int n;
    scanf("%d", &n);
    for (int i = 1; i < NUM_YDAYS; ++i) {
        struct tm check;
        memset(&check, 0, sizeof(check));
        check.tm_year = n - 1900;
        check.tm_mday = i;
        check.tm_isdst = -1;
        mktime(&check);
        if (check.tm_mday % BAD_DAY != 0 && check.tm_wday == THURSDAY && 
                ((check.tm_mday > 21 && check.tm_mday <= 28) 
                || (check.tm_mday > 7 && check.tm_mday <= 14))) {
            printf("%d %d\n", check.tm_mon + 1, check.tm_mday);
        }
    }
    return 0;
}
