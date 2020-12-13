#include <stdio.h>
#include <time.h>
#include <string.h>

enum
{
    ERA_START = 1900,
    DAY_SEC = 86400,
    YEAR_DAY = 360,
    MONTH_DAY = 30
};

void calc_print_date(unsigned long long diff)
{
    unsigned long long year, month, day;
    diff /= DAY_SEC;
    year = diff / YEAR_DAY + 1;
    diff %= YEAR_DAY;
    month = diff / MONTH_DAY + 1;
    diff %= MONTH_DAY;
    day = diff + 1;
    printf("%llu %llu %llu\n", year, month, day);
}

int
main(void)
{
    struct tm new_cal;
    memset(&new_cal, 0, sizeof(new_cal));
    new_cal.tm_year = 25;
    new_cal.tm_mday = 7;
    new_cal.tm_mon = 9;
    new_cal.tm_isdst = -1;
    unsigned long long start_time = timegm(&new_cal);
    unsigned int year, day, month;
    while (scanf("%u%u%u", &year, &month, &day) == 3) {
        struct tm old_cal;
        memset(&old_cal, 0, sizeof(old_cal));
        old_cal.tm_year = year - ERA_START;
        old_cal.tm_mon = month - 1;
        old_cal.tm_mday = day;
        old_cal.tm_isdst = -1;
        unsigned long long old_time = timegm(&old_cal);
        calc_print_date(old_time - start_time);
    }
    return 0;
}
