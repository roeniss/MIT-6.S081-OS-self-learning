#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int main(int argc, char *argv[])
{
    char *argv2[MAXARG];
    char buf[512];
    int i = 0, j;

    argc = MIN(argc, MAXARG - 1);

    if (argc < 2)
    {
        printf("err: command argument is missing\n");
        exit(1);
    }

    while (read(0, buf + i, sizeof(char)) > 0)
    {
        if (*(buf + i) == '\n')
        {
            *(buf + i) = '\0';
            argv2[argc - 1] = buf;

            for (j = 1; j < argc; j++)
            {
                char *arg = malloc(sizeof(char) * (strlen(argv[j]) + 1));
                strcpy(arg, argv[j]);
                argv2[j - 1] = arg;
            }

            if (fork() == 0)
                exec(argv[1], argv2);
            else
                wait(0);
        }
        i++;
    }

    exit(0);
}
