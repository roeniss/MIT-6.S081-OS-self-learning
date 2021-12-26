#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
print_prime(int *pin)
{
    int n, pout[2], buf;
    pipe(pout);
    close(pin[1]);

    if(read(pin[0], &n, 4))
    {
        printf("prime %d\n", n);

        if(fork() == 0)
        {
            print_prime(pout);
            exit(0);
        }
        else{
            close(pout[0]);
            while(read(pin[0], &buf, 4)){
                if(buf % n != 0){
                    write(pout[1], &buf, 4);
                }
            }
            close(pout[1]);
            wait(0);
        }
    }
    close(pin[1]);
    exit(0);
}

int
main(int argc, char *argv[])
{
    int pin[2];
    pipe(pin);
    
    if(fork() == 0)
    {
        print_prime(pin);
        exit(0);
    }
    else
    {
        for (int i = 2; i <= 35; i++)
        {
            write(pin[1], &i, 4);
        }
        close(pin[1]);
    }
    wait(0);
    exit(0);
}

