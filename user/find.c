#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    // 较之ls函数，此处无需填充空格
    return p;
}

void find(char *root, char *path)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // 检查是否存在root路径
    if ((fd = open(root, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", root);
        return;
    }

    // 获取root路径的stat信息，用于判断其是一个文件 or 目录
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", root);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        // 搜索路径是文件，则比对名称是否匹配
        if (strcmp(fmtname(root), path) == 0)
        {
            printf("%s\n", root);
        }
        break;

    case T_DIR:
        // 搜索路径是目录，则需递归搜索
        if (strlen(root) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: root path too long\n");
            break;
        }
        strcpy(buf, root);
        p = buf + strlen(buf);
        *p++ = '/';
        // Directory is a file containing a sequence of dirent structures.
        // 此处挨个读取sizeof(de)，即可获取当前root路径下的子路径
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, path);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Argument Error!!");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}