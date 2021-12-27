#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
check_file(char *path)
{
    
}

// return slash-trimmed name
char*
trimmed_name(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void find_file(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // printf("[DEBUG] path: %s, filename: %s\n", path, filename);

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if(strcmp(trimmed_name(path), filename) == 0)
        {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p = '/';
        p++;

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            char *t_name = trimmed_name(p);
            if(strcmp(t_name, ".") == 0 || strcmp(t_name, "..") == 0)
            {
                continue;
            }
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            find_file(buf, filename);
        }
        break;
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("err: need name and target directory parameters\n");
        exit(1);
    }

    find_file(argv[1], argv[2]);
    exit(0);
}