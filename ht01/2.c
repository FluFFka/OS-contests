#include <stdio.h>
#include <stdlib.h>

enum
{
    MUL = 1103515245,
    INC = 12345
};
const unsigned MOD = 1U << 31;    //2^31

typedef struct RandomGenerator
{
    struct RandomOperations *ops;
    int value;
} RandomGenerator;

struct RandomOperations
{
    int (*next)(RandomGenerator *);
    void (*destroy)(RandomGenerator *);
};

int
next(RandomGenerator *rgen)
{
    rgen->value = ((1LL * MUL * rgen->value % MOD) + INC) % MOD;
    return rgen->value;
}

void
destroy(RandomGenerator *rgen)
{
    free(rgen->ops);
    free(rgen);
}

RandomGenerator *
random_create(int seed)
{
    RandomGenerator *rgen = calloc(1, sizeof(RandomGenerator));
    rgen->ops = calloc(1, sizeof(struct RandomOperations));
    rgen->ops->next = &next;
    rgen->ops->destroy = &destroy;
    rgen->value = seed;
    return rgen;
}
