#include <stdio.h>
#include <string.h>
#include <time.h>

enum { YEAR = 365 };

int
main(void)
{
    char fdate[11];
    int dely, year, month, day;
    scanf("%d-%d-%d%d", &year, &month, &day, &dely);
    
    struct tm ctime;
    memset(&ctime, 0, sizeof(ctime));
    ctime.tm_year = year - 1900;
    ctime.tm_isdst = -1;
    ctime.tm_mday = day + 365 * dely / 4;
    ctime.tm_mon = month - 1;
    mktime(&ctime);
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 1 + 365 * dely / 4;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 2 + 365 * dely / 4;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }

    strftime(fdate, 11, "%Y-%m-%d", &ctime);
    printf("%s\n", fdate);

    memset(&ctime, 0, sizeof(ctime));
    ctime.tm_year = year - 1900;
    ctime.tm_isdst = -1;
    ctime.tm_mday = day + 365 * dely * 2 / 4;
    ctime.tm_mon = month - 1;
    mktime(&ctime);
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 1 + 365 * dely * 2 / 4;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 2 + 365 * dely * 2 / 4;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }

    strftime(fdate, 11, "%Y-%m-%d", &ctime);
    printf("%s\n", fdate);

    memset(&ctime, 0, sizeof(ctime));
    ctime.tm_year = year - 1900;
    ctime.tm_isdst = -1;
    ctime.tm_mday = day + 365 * dely * 3 / 4;
    ctime.tm_mon = month - 1;
    mktime(&ctime);
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 1 + 365 * dely * 3 / 4;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 2 + 365 * dely * 3 / 4;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }

    strftime(fdate, 11, "%Y-%m-%d", &ctime);
    printf("%s\n", fdate);
    
    memset(&ctime, 0, sizeof(ctime));
    ctime.tm_year = year - 1900;
    ctime.tm_isdst = -1;
    ctime.tm_mday = day + 365 * dely;
    ctime.tm_mon = month - 1;
    mktime(&ctime);
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 1 + 365 * dely;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }
    if (ctime.tm_wday == 6 || ctime.tm_wday == 0) {
        memset(&ctime, 0, sizeof(ctime));
        ctime.tm_year = year - 1900;
        ctime.tm_isdst = -1;
        ctime.tm_mday = day + 2 + 365 * dely;
        ctime.tm_mon = month - 1;
        mktime(&ctime);
    }

    strftime(fdate, 11, "%Y-%m-%d", &ctime);
    printf("%s\n", fdate);


    return 0;
}
