#include <stdio.h>
#include <string.h>

int
parse_rwx_permissions(const char *str)
{
    char pattern[] = "rwxrwxrwx";
    if (!str) {
        return -1;
    }
    int len = strlen(str);
    if (len + 1 != sizeof(pattern)) {
        return -1;
    }
    unsigned int mode = 0;
    for (int i = 0; i < len; ++i) {
        if (pattern[i] != str[i] && str[i] != '-') {
            return -1;
        }
        mode <<= 1;
        if (str[i] != '-') {
            mode |= 1;
        }
    }
    return mode;
}
