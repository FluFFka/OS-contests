#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

enum
{
    A_MASK = 0xffffff,
    B_MASK = 0xffff,
    C_MASK = 0xff
};

int
main(int argc, char *argv[])
{
    key_t key = ftok(".", 's');
    int semid = semget(key, 4, 0666 | IPC_CREAT | IPC_EXCL);
    semctl(semid, 4, SETVAL, 0);
    int shmid = shmget(key, 3 * sizeof(unsigned), 0666 | IPC_CREAT);
    int *shmaddr = shmat(shmid, NULL, 0);
    if (!fork()) {
        while (1) {
            struct sembuf sops;
            sops.sem_flg = 0;
            sops.sem_op = -1;
            sops.sem_num = 0;
            if (semop(semid, &sops, 1) == -1) {
                _exit(0);
            }
            int addr = shmaddr[0];
            printf("1 %x\n", addr & A_MASK);
            fflush(stdout);
            sops.sem_flg = 0;
            sops.sem_op = 1;
            sops.sem_num = 4;
            semop(semid, &sops, 1);
        }
        _exit(0);
    }
    if (!fork()) {
        while (1) {
            struct sembuf sops;
            sops.sem_flg = 0;
            sops.sem_op = -1;
            sops.sem_num = 1;
            if (semop(semid, &sops, 1) == -1) {
                _exit(0);
            }
            int addr = shmaddr[1];
            printf("2 %x\n", addr & B_MASK);
            fflush(stdout);
            sops.sem_flg = 0;
            sops.sem_op = 1;
            sops.sem_num = 4;
            semop(semid, &sops, 1);
        }

        _exit(0);
    }
    if (!fork()) {
        while (1) {
            struct sembuf sops;
            sops.sem_flg = 0;
            sops.sem_op = -1;
            sops.sem_num = 2;
            if (semop(semid, &sops, 1) == -1) {
                _exit(0);
            }
            int addr = shmaddr[2];
            printf("3 %x\n", addr & C_MASK);
            fflush(stdout);
            sops.sem_flg = 0;
            sops.sem_op = 1;
            sops.sem_num = 4;
            semop(semid, &sops, 1);
        }
        _exit(0);
    }
    unsigned addr;
    while (scanf("%o", &addr) == 1) {
        struct sembuf sops;
        sops.sem_flg = 0;
        sops.sem_op = 1;
        if (((1U << 31) & addr) == 0) {
            sops.sem_num = 0;
            //type A
        } else if (((1U << 30) & addr) == 0) {
            //type B
            sops.sem_num = 1;
        } else if (((1U << 29) & addr) == 0) {
            //type C
            sops.sem_num = 2;
        } else {
            continue;
        }
        shmaddr[sops.sem_num] = addr;
        semop(semid, &sops, 1);
        sops.sem_flg = 0;
        sops.sem_op = -1;
        sops.sem_num = 4;
        semop(semid, &sops, 1);
    }
    semctl(semid, 3, IPC_RMID, 0);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    while (wait(NULL) != -1);
    return 0;
}
