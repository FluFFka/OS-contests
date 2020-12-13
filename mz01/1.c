#include <stdio.h>

int
main(void)
{
    double x, y;
    scanf("%lf%lf", &x, &y);
    printf("%d\n", x >= 1 && y <= 5 && y <= 10 - x && x <= 7 && y >= 2);
    return 0;
}
