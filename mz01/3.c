#include <stdio.h>

enum
{
    MAXN = 2000
};

int
fast_pow(int num, int pow, int mod)
{
    int res = 1;
    while (pow > 0) {
        if (pow % 2 == 1) {
            res = res * num % mod;
        }
        num = num * num % mod;
        pow /= 2;
    }
    return res;
}

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (n <= 1 || n >= MAXN) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    int pows[MAXN];
    for (int i = 0; i < n; ++i) {
        pows[i] = fast_pow(i, n - 2, n);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            printf("%d ", i * pows[j] % n);
        }
        printf("\n");
    }
    return 0;
}
