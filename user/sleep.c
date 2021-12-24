#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc < 2){
        printf("err: Time argument not provided\n");
        exit(1);
    }

    int t = atoi(argv[1]);
    sleep(t);
    
    exit(0);
}
