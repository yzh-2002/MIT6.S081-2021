#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    // 参数指针数组
    char *param[MAXARG] = {0};
    for (int i = 1; i < argc; i++)
    {
        param[i - 1] = argv[i];
    }
    // 从标准输入中逐个读取，获取剩余的参数
    int arg_cnt = argc - 1;
    int arg_start = 0; // 标识当前参数在lines中存储的起始位置
    int arg_end = 0;   // 末尾位置，也是下一个参数的起始位置-1
    char lines[512];
    char *p = lines;
    char ch;
    while (read(0, &ch, 1))
    {
        if (ch == '\n')
        {
            *p++ = '\0';
            arg_end++;
            param[arg_cnt] = &lines[arg_start];
            arg_start = arg_end + 1;
            // 行结束，需要执行命令
            if (fork() > 0)
            {
                wait(0);
                // 清空参数，以待下次执行命令
                for (int i = argc - 1; i <= arg_cnt; i++)
                {
                    param[i] = 0;
                }
                arg_cnt = argc - 1;
            }
            else
            {
                // child
                exec(argv[1], param);
            }
        }
        else if (ch == ' ')
        {
            // 参数结束，将其添加到参数数组中
            *p++ = '\0';
            arg_end++;
            param[arg_cnt] = &lines[arg_start];
            arg_start = arg_end + 1;
            arg_cnt += 1;
        }
        else
        {
            *p++ = ch;
            arg_end++;
        }
    }

    exit(0);
}