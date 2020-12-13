#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        int fl = 1;
        for (int j = 0; j < len; ++j) {
            if (argv[i][j] == argv[i][len - j - 1] && ((argv[i][j] <= 'Z' && argv[i][j] >= 'A') 
                    || (argv[i][j] <= 'z' && argv[i][j] >= 'a'))) {
                continue;
            }
            fl = 0;
            break;
        }
        if (fl && len) {
            printf("%s\n", argv[i]);
        }



    }
    return 0;
}
