#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  *(volatile int*)0x10000000 = 1;
  exit(0);
}
