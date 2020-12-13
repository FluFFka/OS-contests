#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <inttypes.h>

enum
{
    MESSAGE_SIZE = 2 * sizeof(int64_t) + 1
};

struct Msgbuf
{
    long mtype;
    char mtext[MESSAGE_SIZE]; 
};

int
main(int argc, char *argv[])
{
    if (argc < 6) {
        return 1;
    }
    key_t key = atoi(argv[1]);
    int n = atoi(argv[2]);
    int64_t value1, value2, maxval;
    sscanf(argv[3], "%" SCNd64, &value1);
    sscanf(argv[4], "%" SCNd64, &value2);
    sscanf(argv[5], "%" SCNd64, &maxval);
    int msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
    for (int i = 0; i < n; ++i) {
        int pid = fork();
        if (pid == -1) {
            msgctl(msgid, IPC_RMID, NULL);
            while (wait(NULL) != -1);
            return 1;
        }
        if (!pid) {
            while (1) {
                struct Msgbuf currmsg;
                int ret = msgrcv(msgid, &currmsg, sizeof(currmsg.mtext), i + 1, 0);
                if (ret == -1) {
                    _exit(0);
                }
                int64_t x1, x2, x3;
                sscanf(currmsg.mtext, "%" SCNd64 "%" SCNd64, &x1, &x2);
                x3 = x1 + x2;
                printf("%d %" PRId64 "\n", i, x3);
                fflush(stdout);
                if (x3 > maxval) {
                    msgctl(msgid, IPC_RMID, NULL);
                    _exit(0);
                }
                currmsg.mtype = (x3 % n) + 1;
                sprintf(currmsg.mtext, "%" PRId64 " %" PRId64, x2, x3);
                msgsnd(msgid, &currmsg, sizeof(currmsg.mtext), 0);
            }
            _exit(0);
        }
    }
    struct Msgbuf currmsg;
    currmsg.mtype = 1;
    sprintf(currmsg.mtext, "%" PRId64 " %" PRId64, value1, value2);
    msgsnd(msgid, &currmsg, sizeof(currmsg.mtext), 0);
    while (wait(NULL) != -1);
    return 0;
}
