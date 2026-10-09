#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  lastwords("writing config file, step 3");
  *(volatile int*)0x10000000 = 1;   // unmapped address, so this faults
  exit(0);
}