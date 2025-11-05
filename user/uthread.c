#include "types.h"
#include "user.h"
#include "uthread.h"
#include "stdlib.h"

int thread_create(void (*start)(void*), void *arg, void *stack)
{
  return clone(start, arg, stack);
}

int thread_join(int *ret)
{
  return join(ret);
}
