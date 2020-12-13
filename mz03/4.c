#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>

enum { BUFFER_SIZE = 16 };

int
main(int argc, char *argv[])
{
    uint64_t res = 0, curr = 0;
    char buf[BUFFER_SIZE];
    int curr_size = 0, sig = 1;
    while ((curr_size = read(0, buf, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < curr_size; ++i) {
            if (isspace((unsigned char) buf[i])) {
                if (sig > 0) {
                    res += curr;
                } else {
                    res -= curr;
                }
                curr = 0;
                sig = 1;
            } else {
                if (buf[i] == '-') {
                    sig = -1;
                } else if (buf[i] == '+') {
                    sig = 1;
                } else {
                    curr = curr * 10 + buf[i] - '0';
                }
            }
        }
    }
    printf("%" PRId64 "\n", res);

    return 0;
}
