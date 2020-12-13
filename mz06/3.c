#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

enum
{
    START_SIZE = 4,
    BACK_SIZE = 3
};

void
normalize(const char *path1, char *rpath1)
{
    int ind = 0, rind = 0;
    for (; path1[ind]; ++ind, ++rind) {
        rpath1[rind] = path1[ind];
        if (path1[ind] == '.' && ind - 2 >= 0 && path1[ind - 1] == '.' && path1[ind - 2] == '/'
                && (!path1[ind + 1] || path1[ind + 1] == '/')) {
            rind -= 3;
            for (int i = rind; i >= 0; --i) {
                rind = i - 1;
                if (rpath1[i] == '/') {
                    break;
                }
            }
        } else if (path1[ind] == '.' && ind - 1 >= 0 && path1[ind - 1] == '/'
                && (!path1[ind + 1] || path1[ind + 1] == '/')) {
            rind = rind - 2;
        }
        if (rind < 0) {
            rind = -1;
        }
    }
    rpath1[rind] = 0;
}

char *relativize_path(const char *path1, const char *path2)
{
    char *rpath1 = calloc(strlen(path1) + 1, sizeof(rpath1[0]));
    char *rpath2 = calloc(strlen(path2) + 1, sizeof(rpath2[0]));
    normalize(path1, rpath1);
    normalize(path2, rpath2);
    int p_fork = 0, j = 0;
    for (; rpath1[j] == rpath2[j] && rpath1[j]; ++j) {
        if (rpath1[j] == '/') {
            p_fork = j;
        }
    }
    if (!rpath2[j] && rpath1[j] == '/') {
        p_fork = j;
    }
    size_t sz = START_SIZE, taken = 1;
    char *path = calloc(sz, sizeof(path[0]));
    for (int i = p_fork + 1; rpath1[i]; ++i) {
        if (rpath1[i] == '/') {
            if (taken + BACK_SIZE > sz) {
                sz *= 2;
                path = realloc(path, sz * sizeof(path[0]));
                if (!path) {
                    fprintf(stderr, "Can't allocate memory\n");
                    return NULL;
                }
            }
            path = strcat(path, "../");
            taken += BACK_SIZE;
        }
    }
    if (rpath2[p_fork] && strcmp(path2, "/")) {
        size_t need = strlen(rpath2 + p_fork + 1);
        if (taken + need > sz) {
            sz = taken + need;
            path = realloc(path, sz * sizeof(path[0]));
            if (!path) {
                fprintf(stderr, "Can't allocate memory\n");
                return NULL;
            }
        }
        taken += need;
        path = strcat(path, rpath2 + p_fork + 1);
    } else if (taken == 1) {
        path = strcat(path, ".");
        taken = 2;
    } else {
        taken--;
    } 
    path[taken - 1] = 0;
    path = realloc(path, taken * sizeof(path[0]));
    free(rpath1);
    free(rpath2);
    return path;
}
