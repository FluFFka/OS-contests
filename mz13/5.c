#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void
getarrayintersection(int **data, int *sizes, int start, int end, int fd)
{
    int ind1 = 0, ind2 = 0;
    while (ind1 < sizes[start] && ind2 < sizes[end]) {
        if (data[start][ind1] == data[end][ind2]) {
            write(fd, &data[start][ind1], sizeof(int));
            ind1++;
            ind2++;
        } else if (data[start][ind1] > data[end][ind2]) {
            ind2++;
        } else {
            ind1++;
        }
    }
}

void
getpipeintersection(int fd1, int fd2, int fd)
{
    int curr1, curr2;
    if (read(fd1, &curr1, sizeof(int)) != sizeof(int)) {
        return;
    }
    if (read(fd2, &curr2, sizeof(int)) != sizeof(int)) {
        return;
    }
    while (1) {
        if (curr1 == curr2) {
            write(fd, &curr1, sizeof(int));
            if (read(fd2, &curr2, sizeof(int)) != sizeof(int)) {
                return;
            }
            if (read(fd1, &curr1, sizeof(int)) != sizeof(int)) {
                return;
            }
        } else if (curr1 > curr2) {
            if (read(fd2, &curr2, sizeof(int)) != sizeof(int)) {
                return;
            }
        } else {
            if (read(fd1, &curr1, sizeof(int)) != sizeof(int)) {
                return;
            }
        }
    }
}

void
intersection(int **data, int *sizes, int start, int end, int fd)  //returns sequence in pipe
{
    if (start == end) {
        for (int i = 0; i < sizes[start]; ++i) {
            write(fd, &data[start][i], sizeof(int));
        }
        return;
    }
    if (start + 1 == end) {
        getarrayintersection(data, sizes, start, end, fd);
        return;
    }
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    if (!fork()) {
        close(p2[1]);
        close(p2[0]);
        close(p1[0]);
        intersection(data, sizes, start, (start + end) / 2, p1[1]);
        close(p1[1]);
        _exit(0);
    }
    close(p1[1]);
    if (!fork()) {
        close(p1[0]);
        close(p2[0]);
        intersection(data, sizes, (start + end) / 2 + 1, end, p2[1]);
        close(p2[1]);
        _exit(0);
    }
    close(p2[1]);
    getpipeintersection(p1[0], p2[0], fd);
}

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        return 0;
    }
    //I should start input data in pipe from start, but I finally invent a solve solve task too late
    //Start input data to pipe and function intersection would use only function getpipeintersection,
    //    so you can delete function getarrayintersection
    int **data = calloc(argc - 1, sizeof(int *));
    int *sizes = calloc(argc - 1, sizeof(int));
    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "r");
        int num;
        int size = 1, curr_ind = 0;
        int *curr = calloc(size, sizeof(int));
        while (fscanf(file, "%d", &num) == 1) {
            if (curr_ind == size) {
                size *= 2;
                curr = realloc(curr, size * sizeof(int));
                if (!curr) {
                    fprintf(stderr, "Can't allocate memory\n");
                    return 1;
                }
            }
            curr[curr_ind] = num;
            curr_ind++;
        }
        if (curr_ind != 0) {
            curr = realloc(curr, curr_ind * sizeof(int));
            if (!curr) {
                fprintf(stderr, "Can't reallocate memory\n");
                return 1;
            }
        }
        data[i - 1] = curr;
        sizes[i - 1] = curr_ind;
    }
    int fd[2];
    pipe(fd);
    intersection(data, sizes, 0, argc - 2, fd[1]);
    close(fd[1]);
    int num;
    while (read(fd[0], &num, sizeof(int))) {
        printf("%d\n", num);
    }
    close(fd[0]);
    
    
    free(sizes);
    for (int i = 0; i < argc - 1; ++i) {
        free(data[i]);
    }
    free(data);

    return 0;
}
