#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"
int
readline(char*new_argv[32], int cur_argc) {
    char buf[512];
    int n = 0;
    while(read(0, buf+n, 1))
    {
        if(n == 511)
        {
            fprintf(2, "argument is too long\n");
            exit(1);
        }
        if(buf[n] == '\n')
            break;
        n++;
    }
    // zero terminated
    buf[n] = 0;
    if(n == 0) return 0;
    int offset = 0;
    // fprintf(1, "before offset loop\n");
    while(offset < n) {
        new_argv[cur_argc++] = offset + buf;
        while(buf[offset] != ' ' && offset < n)
            offset++;
        while(buf[offset] == ' ' && offset < n)
            new_argv[offset++] = 0;
    }
    // fprintf(1, "not readline\n");
    return cur_argc;
}
void 
xargs(int argc, char *argv[], char* cmd){
    char *arg_arr[MAXARG];  // need allocate memory space for each pointer in array
    for(int i=0; i<argc; i++)
    {
        arg_arr[i] = malloc(strlen(argv[i]) + 1); // extra one for zero terminated 
        strcpy(arg_arr[i], argv[i]);
    }
    // fprintf(1, "\nnumber of arg: %d\n", argc);
    int cur_argc;
    // run command using input from previous read line
    while((cur_argc = readline(arg_arr, argc)) != 0)
    {
    //     fprintf(1, "%d\n", cur_argc);
        // handle the last argu with zero terminated
        arg_arr[cur_argc] = 0;
        if(fork() == 0)
        {
            exec(cmd, arg_arr);
            // not reach below
            fprintf(2, "exec failed\n");
            exit(1);
        } else {
            wait((int*) 0);
        }
    }
    return;
}

int
main(int argc, char *argv[]) {
    if(argc < 2)
        exit(0);
    // argc - 1 and argv + 1 is for 
    // not to bring xargs itself to arguments
    xargs(argc - 1, argv + 1, argv[1]);
    exit(0);
}