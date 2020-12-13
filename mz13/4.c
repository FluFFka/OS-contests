#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }
    int count = atoi(argv[1]);
    int key = ftok(".", 's');
    int semid = semget(key, count, 0666 | IPC_CREAT | IPC_EXCL);
    semctl(semid, count, SETVAL, 0);
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    for (int i = 0; i < count; ++i) {
        if (!fork()) {
            while (1) {
                struct sembuf sops;
                sops.sem_flg = 0;
                sops.sem_op = -1;
                sops.sem_num = i;
                if (semop(semid, &sops, 1) == -1) {
                    _exit(0);
                }
                int num;
                if (scanf("%d", &num) == 1) {
                    printf("%d %d\n", i, num);
                } else {
                    semctl(semid, count, IPC_RMID, 0);
                    _exit(0);
                }
                sops.sem_flg = 0;
                sops.sem_op = 1;
                if (num < 0) {
                    num += ((-1) * num / count + 1) * count;
                }
                sops.sem_num = num % count;
                semop(semid, &sops, 1);
            }
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
