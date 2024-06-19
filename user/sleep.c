#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // argv[0] 一般是命令本身，e.g. "./sleep"
  if (argc!=2){
    printf("Argument Error!!");
  }
  sleep(atoi(argv[1]));
  exit(0);
}