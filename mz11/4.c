#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>

// #define _POSIX_C_SOURCE 199309L

volatile sig_atomic_t signal_got = 0;

void
handler(int signal)
{
    signal_got = 1;
}

void wait_signal(const sigset_t *old_mask)
{
    while (!signal_got) {
        sigsuspend(old_mask);
    }
    signal_got = 0;
}

void
child_work(FILE *filein, FILE *fileout, int max_n, pid_t opponent_pid,
        const sigset_t *old_mask, int player_num)
{
    int curr;
    while (fscanf(filein, "%d", &curr) && curr < max_n) {
        printf("%d %d\n", player_num, curr);
        fflush(stdout);
        fprintf(fileout, "%d ", curr + 1);
        fflush(fileout);
        kill(opponent_pid, SIGUSR1);
        wait_signal(old_mask);
    }
    if (curr == max_n) {
        fprintf(fileout, "%d ", curr);
        fflush(fileout);
        fclose(filein);
        fclose(fileout);
        _exit(0);
    }
}


int
main(int argc, char *argv[])
{
    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIGUSR1, &sig, NULL);
    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    int max_n = atoi(argv[1]);
    int fd[2];
    pipe(fd);
    FILE *filein = fdopen(fd[0], "r");
    FILE *fileout = fdopen(fd[1], "w");
    int pid1 = fork();
    if (pid1 == 0) {
        wait_signal(&old_mask);
        fscanf(filein, "%d", &pid1);
        child_work(filein, fileout, max_n, pid1, &old_mask, 1);
    }
    int pid2 = fork();
    if (pid2 == 0) {
        wait_signal(&old_mask);
        child_work(filein, fileout, max_n, pid1, &old_mask, 2);
    }
    fprintf(fileout, "%d %d ", pid2, 1);
    fclose(filein);
    fclose(fileout);
    kill(pid1, SIGUSR1);
    wait(NULL);
    if (max_n % 2 == 0) {
        kill(pid1, SIGUSR1);    
    } else {
        kill(pid2, SIGUSR1);
    }
    wait(NULL);
    printf("Done\n");
    return 0;
}
