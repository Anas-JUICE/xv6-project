#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// sys_clone wrapper
uint64
sys_clone(void)
{
  uint64 fcn, arg, stack;
  if(argaddr(0, &fcn) < 0 || argaddr(1, &arg) < 0 || argaddr(2, &stack) < 0)
    return -1;
  return clone((void*)fcn, (void*)arg, (void*)stack);
}

// sys_join wrapper
uint64
sys_join(void)
{
  uint64 addr;
  if(argaddr(0, &addr) < 0)
    return -1;
  return join((int*)addr);
}
