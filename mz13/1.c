#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>

void
proc(int *shmaddr, int num, int semid, int nproc, int maxval)
{
    while (1) {
        struct sembuf sops;
        sops.sem_flg = 0;
        sops.sem_op = -1;
        sops.sem_num = num - 1;
        if (semop(semid, &sops, 1) == -1) {
            _exit(0);
        }
        printf("%d %d %d\n", num, shmaddr[2 * (num - 1)], shmaddr[2 * (num - 1) + 1]);
        fflush(stdout);
        int val = shmaddr[2 * (num - 1)];
        if (1LL * val + 1 > maxval) {
            _exit(0);
        }
        val++;
        int next = (((((1LL * val * val) % nproc) * val) % nproc) * val) % nproc;
        shmaddr[2 * next] = val;
        shmaddr[2 * next + 1] = num;
        sops.sem_flg = 0;
        sops.sem_op = 1;
        sops.sem_num = next;
        semop(semid, &sops, 1);
    }
}

int
main(int argc, char *argv[])
{
    int nproc = atoi(argv[1]), key = atoi(argv[2]), maxval = atoi(argv[3]);
    int shmid = shmget(key, 2 * nproc * sizeof(int), 0666 | IPC_CREAT);
    int *shmaddr = shmat(shmid, NULL, 0);
    int semid = semget(key, nproc, 0666 | IPC_CREAT | IPC_EXCL);
    semctl(semid, nproc, SETVAL, (int) 0);
    for (int i = 0; i < nproc; ++i) {
        if (!fork()) {
            proc(shmaddr, i + 1, semid, nproc, maxval);
        }
    }
    shmaddr[0] = 0; //val
    shmaddr[1] = 0; //rec
    struct sembuf sops;
    sops.sem_flg = 0;
    sops.sem_op = 1;
    sops.sem_num = 0;
    semop(semid, &sops, 1);
    wait(NULL);
    semctl(semid, nproc, IPC_RMID, (int) 0);
    while (wait(NULL) != -1);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
