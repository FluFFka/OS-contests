#include <stdio.h>
#include <math.h>

enum
{
    MAX_PROCENT = 100,
    ROUND_NUMBER = 10000
};

int
main(int argc, char *argv[])
{
    double value;
    sscanf(argv[1], "%lf", &value);
    for (int i = 2; i < argc; ++i) {
        double procent;
        sscanf(argv[i], "%lf", &procent);
        value *= procent / MAX_PROCENT + 1;
        value = round(value * ROUND_NUMBER) / ROUND_NUMBER;
    }
    printf("%.4f\n", value);
    return 0;
}
