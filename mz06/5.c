#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    STATUS_OFF = 3,
    MODE_MASK = 07
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};


static int
is_supervisor(const struct Task *task)
{
    return task->uid == 0;
}

static int
is_owner(const struct stat *stb, const struct Task *task)
{
    return stb->st_uid == task->uid;
}

static int
is_in_group(const struct stat *stb, const struct Task *task)
{
    for (int i = 0; i < task->gid_count; ++i) {
        if (stb->st_gid == (task->gids)[i]) {
            return 1;
        }
    }
    return 0;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (is_supervisor(task)) {
        return 1;
    }
    int status;
    if (is_owner(stb, task)) {
        status = 2;
    } else if (is_in_group(stb, task)) {
        status = 1;
    } else {
        status = 0;
    }
    int check_mode = (stb->st_mode >> (STATUS_OFF * status)) & MODE_MASK;
    if ((access & check_mode) == access) {
        return 1;
    }
    return 0;
}
