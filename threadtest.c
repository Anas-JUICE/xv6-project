#include "types.h"
#include "stat.h"
#include "user.h"

void
worker(void *arg)
{
  int id = (int)arg;
  printf(1, "Hello from thread %d\n", id);
  exit();   // end this thread
}

int
main(void)
{
  void *stack1, *stack2;
  int tid1, tid2;

  // allocate two stacks: one page each
  stack1 = malloc(4096);
  stack2 = malloc(4096);

  tid1 = clone(worker, (void*)1, stack1);
  tid2 = clone(worker, (void*)2, stack2);

  printf(1, "clone returned %d and %d in parent\n", tid1, tid2);

  // simple delay so threads print before parent exits
  sleep(50);

  exit();
}