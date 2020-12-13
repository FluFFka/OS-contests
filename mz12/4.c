#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, 0751);
    int mod = atoi(argv[2]);
    FILE *fin = fdopen(fd, "w+");
    fprintf(fin, "#! /usr/bin/env python3\nn=int(input())\nprint((n*(n+1)//2)%%%d)", mod);
    fclose(fin);
    return 0;
}
