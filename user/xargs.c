#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLEN 1024

int
get_args(int new_argc, char *new_argv[])
{
    int n = 0;
    char buf[MAXLEN];

    while(read(0, buf + n, 1)){
        if(buf[n] == '\n')
            break;
        n++;
        if(n >= MAXLEN - 1){
            fprintf(2, "xargs: params too long\n");
            exit(1);
        }
    }
    buf[n] = 0;

    //there is no output of last command.
    if(n == 0)  return 0;

    //split by space
    int offset = 0;
    while(offset < n){
        while(buf[offset] == ' ' && offset < n)
            offset++;

        int split = offset;

        while(buf[offset] != ' ' && offset < n)
            offset++;
        buf[offset++] = 0;

        new_argv[new_argc++] = buf + split;
    }

    return new_argc;
}

int
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "Usage: xargs command [params ..]\n");
        exit(1);
    }

    char *cmd = argv[1];
    int new_argc = 0;
    char *new_argv[MAXARG];

    for(int i = 1; i < argc; i++)
        new_argv[new_argc++] = argv[i];

    while((new_argc = get_args(argc - 1, new_argv))){
        new_argv[new_argc] = 0;
        if(fork() == 0){
            exec(cmd, new_argv);
            fprintf(2, "xargs: exec error\n");
            exit(1);
        } else {
            wait(0);
        }
    }

    exit(0);
}