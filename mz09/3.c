#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int
run(const char *str)
{
    if (!fork()) {
        execlp(str, str, NULL);
        _exit(1);
    }
    int st;
    wait(&st);
    return WIFEXITED(st) && (WEXITSTATUS(st) == 0);
}

int
main(int argc, char *argv[])
{
    return ((run(argv[1]) || run(argv[2])) && run(argv[3])) == 0;
}
