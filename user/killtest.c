#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  printf("killtest running, pid %d\n", getpid());
  lastwords("waiting forever in loop");
  for(;;)
    pause(10);
  exit(0);
}