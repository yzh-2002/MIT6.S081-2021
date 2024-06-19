#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid > 0) //child pid
    {
        // parent
        int p_pid =getpid();
        char content[6];
        write(p[0],"Hello",5);
        wait((int *)0);
        read(p[1],content,5);
        printf("%d: received pong\n",p_pid);
        close(p[1]);
    }
    else if (pid == 0)
    {
        // child
        int c_pid =getpid();
        char content[6];
        read(p[1],content,5);
        printf("%d: received ping\n",c_pid);
        write(p[0],"Hello",5);
        close(p[0]);
        exit(0);
    }
    else
    {
        printf("fork error\n");
    }
    exit(0);
}