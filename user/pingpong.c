#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

int
main(int argc, char *argv[])
{
    int pid;
    int p[2];
    pipe(p);

    if(fork() == 0){
        //child
        pid = getpid();
        char buf[2];
        if(read(p[READ], buf, 1) != 1){
            fprintf(2, "pingpong: read error in child\n");
            exit(1);
        }
        close(p[READ]);
        printf("%d: received ping\n", pid);
        if(write(p[WRITE], buf, 1) != 1){
            fprintf(2, "pingpong: write error in child\n");
            exit(1);
        }
        close(p[WRITE]);
        exit(0);
    } else {
        //parent
        pid = getpid();
        char buf[2];
        if(write(p[WRITE], "a", 1) != 1){
            fprintf(2, "pingpong: write error in parent\n");
            exit(1);
        }
        close(p[WRITE]);
        wait(0);
        if(read(p[READ], buf, 1) != 1){
            fprintf(2, "pingpong: read error in parent\n");
            exit(1);
        }
        printf("%d: received pong\n", pid);
        close(p[READ]);
        exit(0);
    }
}