#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2]; // read, write
    int buf[1];
    pipe(p1);
    
    if (fork() == 0) // child
    {
        read(p1[0], buf, sizeof(buf));
        close(p1[0]);

        printf("%d: received ping\n", getpid());
        
        buf[0] = getpid();
        write(p1[1], buf, sizeof(buf));
        close(p1[1]);
        exit(0);
    }
    else
    {
        buf[0] = getpid();
        write(p1[1], buf, sizeof(buf));
        close(p1[1]);

        read(p1[0], buf, sizeof(buf));
        close(p1[0]);

        printf("%d: received pong\n", getpid());
    }
    exit(0);
}
