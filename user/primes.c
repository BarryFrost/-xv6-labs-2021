#include "kernel/types.h"
#include "user/user.h"

void new_proc(int p[2]) {
    int prime, flag, n;
    close(p[1]);
    if(read(p[0], &prime, sizeof(prime)) != sizeof(prime))
    {
        fprintf(2, "error while read from pipe\n");
        exit(1);
    }
    fprintf(1, "prime %d\n", prime);
    flag = read(p[0], &n, sizeof(n));
    if(flag) {
        int new_pipe[2];
        pipe(new_pipe);
        if(fork() == 0) {
            new_proc(new_pipe);
        } else {
            close(new_pipe[0]);
            if(n % prime != 0) // not factor of prime
                write(new_pipe[1], &n, sizeof(n));
            while(read(p[0], &n, sizeof(n))) {
                if(n % prime != 0) // not factor of prime
                    write(new_pipe[1], &n, sizeof(n));
            }
            close(p[0]);
            close(new_pipe[1]);
            wait((int*) 0);
        }
    }
}

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);

    // if number cannot divid by i
    //  create a pipe and send number to it
    if(fork() == 0) {
        new_proc(fd);
    } else {    // parent
        close(fd[0]);
        for(int i = 2; i <= 35; i++)
        if(write(fd[1], &i, sizeof(i)) != sizeof(i))
        {
            fprintf(2, "error while write to pipe\n");
            exit(1);
        }
        close(fd[1]);
        wait((int*) 0);
    }
    exit(0);
}