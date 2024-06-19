## Lab1

### xargs

作用：将标准输入转换为命令行参数。

exec函数签名：`int exec(char *path, char **argv)`，该函数会读取指定的path路径并从中加载指令，替代当前调用进程的指令（相当于丢弃了调用进程的内存，并开始执行新加载的指令），argv为命令行参数。

```c
int main(){
    char *argv[] = {"echo","this","is","echo",0};
    exec("echo",argv);
    // exec不会返回，因为其会完全替换当前进程的内存
    // 只有当exec调用出错时返回-1，比如：找不到指定path对应信息
    printf("echo failed!\n");
}
```

c语言中为字符串数组赋值的常见方法：
```c
int main(int argc,char* argv){
    char *param[32];
}
```

## Reference

1. [xargs 命令教程](https://www.ruanyifeng.com/blog/2019/08/xargs-tutorial.html)
