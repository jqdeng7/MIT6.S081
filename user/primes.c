#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

void
child(int pl[2])
{
    int n;
    int prime;
    int pr[2];

    close(pl[WRITE]);
    pipe(pr);

    if(read(pl[READ], &prime, sizeof(int)) != sizeof(int))
        exit(1);

    if(fork() == 0){
        child(pr);
    } else {
        close(pr[READ]);
        printf("prime %d\n", prime);
        while(read(pl[READ], &n, sizeof(int)) != 0){
            if(n % prime)
                write(pr[WRITE], &n, sizeof(int));
        }
        close(pl[READ]);
        close(pr[WRITE]);
        wait(0);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if(fork() == 0){
        child(p);
    } else {
        close(p[READ]);
        for(int i = 2; i <= 35; i++){
            if(write(p[WRITE], &i, sizeof(i)) != sizeof(i)){
                fprintf(2, "primes: read error\n");
                exit(1);
            }
        }
        close(p[WRITE]);
        wait(0);
    }
    
    exit(0);
}