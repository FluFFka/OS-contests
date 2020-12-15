#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int
main(int argc, char *argv[])
{
    key_t key = ftok(".", 's');
    int semid = semget(key, argc - 1, 0666 | IPC_CREAT | IPC_EXCL);
    semctl(semid, argc - 1, SETVAL, 0);
    for (int i = 1; i < argc; ++i) {
        if (!fork()) {
            long long curr;
            sscanf(argv[i], "%lld", &curr);
            int sum = 0;
            while (curr) {
                sum += curr % 4;
                curr /= 4;
            }
            if (sum < 0) {
                sum *= (-1);
            }
            struct sembuf sops;
            sops.sem_flg = 0;
            sops.sem_op = -1;
            sops.sem_num = i - 1;
            semop(semid, &sops, 1);
            printf("%d\n", sum);
            fflush(stdout);
            if (i == argc - 1) {
                semctl(semid, argc - 1, IPC_RMID, 0);
                _exit(0);
            }
            sops.sem_flg = 0;
            sops.sem_op = 1;
            sops.sem_num = i;
            semop(semid, &sops, 1);
            _exit(0);
        }
    }
    struct sembuf sops;
    sops.sem_flg = 0;
    sops.sem_op = 1;
    sops.sem_num = 0;
    semop(semid, &sops, 1);
    while (wait(NULL) != -1);
    return 0;
}
