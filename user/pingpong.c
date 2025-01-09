#include "kernel/types.h"
#include "user/user.h"

int
main()
{
    int p[2];
    char ch[1] = {'b'}; // for convenient use for read and write. They need to refer a poniter to buffer
    if(pipe(p) < 0)
    {
        fprintf(2, "open pipe failed\n");
        exit(1);
    }
    // parent send to write pipe
    write(p[1], ch, sizeof(ch));
    // close(p[1]);
    if(fork() == 0)    // child process, 
    {
        // receive from read end of pipe
        // int n = 
        read(p[0], (void*)ch, sizeof(ch));
        close(p[0]);
        int child_pid = getpid();
        // if(n > 0)
        fprintf(1, "%d: received ping\n", child_pid);
        // send to pipe and close
        write(p[1], ch, sizeof(ch));
        close(p[1]);
    } else {        // parent process
        // int child_pid = 
        wait((int*) 0); // wait for child to act
        // fprintf(1, "child %d is done\n", child_pid);
        int parent_pid = getpid();
        // int n = 
        read(p[0], (void *)ch, sizeof(ch));
        close(p[0]);
        // if(n > 0)
        fprintf(1, "%d: received pong\n", parent_pid);
        close(p[1]);
    }

    exit(0);
}