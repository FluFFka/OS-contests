#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int
main(int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    if (!fork()) {
        int fin = open(argv[2], O_RDONLY);
        int fout = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, 0660);
        int ferr = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0660);
        if (fin == -1 || fout == -1 || ferr == -1) {
            _exit(42);
        }
        if (dup2(fin, 0) == -1) {
            _exit(42);
        }
        if (dup2(fout, 1) == -1) {
            _exit(42);
        }
        if (dup2(ferr, 2) == -1) {
            _exit(42);
        }
        if (close(fin) == -1 || close(fout) == -1 || close(ferr) == -1) {
            _exit(42);
        }
        if (execlp(argv[1], argv[1], NULL) == -1) {
            _exit(42);
        }
        _exit(0);
    }
    int st;
    wait(&st);
    printf("%d\n", st);
    return 0;
}
