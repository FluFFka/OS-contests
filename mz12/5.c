#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    BUF_SIZE = 4096,
    CHAR_SIZE = 8
};

int
main(int argc, char *argv[])
{
    
    int pid1, pid2;
    pid1 = fork();
    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGIO);
    sigprocmask(SIG_BLOCK, &mask, &old_mask);
    int sigfd = signalfd(-1, &mask, 0);
    if (pid1 == 0) {
        while (1) {
            unsigned char curr = 0;
            for (int i = 0; i < CHAR_SIZE; ++i) {
                struct signalfd_siginfo sigbuf;
                read(sigfd, &sigbuf, sizeof(sigbuf));
                if (sigbuf.ssi_signo == SIGIO) {
                    _exit(0);
                }
                curr >>= 1;
                if (sigbuf.ssi_signo == SIGUSR2) {
                    curr |= 1U << (CHAR_SIZE - 1);
                }
                kill(sigbuf.ssi_pid, SIGALRM);
            }
            putchar(curr);
            fflush(stdout);
        }
        _exit(0);
    }
    pid2 = fork();
    if (pid2 == 0) {
        int filefd = open(argv[1], O_RDONLY);
        unsigned char buf[BUF_SIZE];
        int sym_read;
        while ((sym_read = read(filefd, buf, sizeof(BUF_SIZE))) > 0) {
            for (int i = 0; i < sym_read; ++i) {
                unsigned char curr = buf[i];
                for (int j = 0; j < CHAR_SIZE; ++j) {
                    if (curr & 1) {
                        kill(pid1, SIGUSR2);
                    } else {
                        kill(pid1, SIGUSR1);
                    }
                    curr >>= 1;
                    struct signalfd_siginfo sigbuf;
                    read(sigfd, &sigbuf, sizeof(sigbuf));
                }
            }
        }
        close(filefd);
        kill(pid1, SIGIO);
        _exit(0);
    }
    while (wait(NULL) != -1);
    return 0;
}
