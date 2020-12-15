#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <limits.h>

enum { DATA_LIMIT = 10 };

int
main(int argc, char *argv[])
{
    if (argc < 6) {
        return 1;
    }
    int count, key, nproc, iter_count, seed;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);
    sscanf(argv[5], "%d", &seed);
    int shmid = shmget(key, count * sizeof(int), 0666 | IPC_CREAT);
    int *shmaddr = shmat(shmid, NULL, 0);
    for (int i = 0; i < count; ++i) {
        int num;
        scanf("%d", &num);
        shmaddr[i] = num;
    }
    int semid = semget(key, count, 0666 | IPC_CREAT);
    for (int i = 0; i < count; ++i) {
        struct sembuf sops;
        sops.sem_flg = 0;
        sops.sem_op = 1;
        sops.sem_num = i;
        semop(semid, &sops, 1);
    }
    for (int i = 0; i < nproc; ++i) {
        if (!fork()) {
            srand(seed);
            for (int j = 0; j < iter_count; ++j) {
                int ind1 = rand() / ((double) RAND_MAX + 1) * count;
                int ind2 = rand() / ((double) RAND_MAX + 1) * count;
                int data = rand() / ((double) RAND_MAX + 1) * DATA_LIMIT;
                struct sembuf sops[2];
                sops[0].sem_flg = 0;
                sops[0].sem_op = -1;
                sops[0].sem_num = ind1;
                sops[1].sem_flg = 0;
                sops[1].sem_op = -1;
                sops[1].sem_num = ind2;
                if (ind1 == ind2) {
                    semop(semid, sops, 1);
                } else {
                    semop(semid, sops, 2);
                }
                operation(shmaddr, ind1, ind2, data);
                sops[0].sem_flg = 0;
                sops[0].sem_op = 1;
                sops[0].sem_num = ind1;
                sops[1].sem_flg = 0;
                sops[1].sem_op = 1;
                sops[1].sem_num = ind2;
                if (ind1 == ind2) {
                    semop(semid, sops, 1);
                } else {
                    semop(semid, sops, 2);
                }
            }
            _exit(0);
        }
    }
    while (wait(NULL) != -1);
    
    semctl(semid, count, IPC_RMID, 0);
    for (int i = 0; i < count; ++i) {
        printf("%d\n", shmaddr[i]);
        fflush(stdout);
    }
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}
