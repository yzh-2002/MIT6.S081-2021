#include "kernel/types.h"
#include "user/user.h"

void filter(int *lp)
{
    // FIXME: 忘记设置退出
    int n;
    if (read(lp[0], &n, 4))
    {
        printf("prime %d\n", n);
    }
    else
    {
        exit(0);
    }
    // 筛选素数
    int rp[2];
    pipe(rp);
    int pid = fork();
    if (pid > 0)
    {
        int num;
        while (read(lp[0], &num, 4))
        {
            if (num % n != 0)
            {
                write(rp[1], &num, 4);
            }
        }
        close(lp[0]);
        close(rp[1]);
        wait(0);
    }
    else if (pid == 0)
    {
        // 父进程关闭lp[0]不影响子进程的lp[0]
        close(lp[0]);
        close(rp[1]);
        filter(rp);
        exit(0);
    }
    exit(0);
}

int main()
{
    // 将2-35写入管道
    int p[2];
    pipe(p);
    for (int i = 2; i <= 35; i++)
    {
        write(p[1], &i, 4);
    }
    // 写入完成之后，及时释放资源
    close(p[1]);
    filter(p);
    exit(0);
}