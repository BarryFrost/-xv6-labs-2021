#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2)    // no argument attached, print usage and exit
    {
        fprintf(2, "Usage: sleep [ms]\n");
        exit(1);
    } else if(argc > 2) {
        fprintf(2, "Too many argument.\n");
        exit(1);
    }
    
    int ms = atoi(argv[1]);
    int ret = sleep(ms);

    exit(ret);
}