#include "types.h"
#include "user.h"
#include "uthread.h"

void
func(void *arg)
{
  int id = *(int*)arg;
  printf("Thread %d starting\n", id);
  for(int i=0;i<100000;i++) ; // busy work
  printf("Thread %d done\n", id);
  exit(0);
}

int
main(int argc, char **argv)
{
  void *stack1 = malloc(4096);
  void *stack2 = malloc(4096);
  int id1 = 0, id2 = 1;

  if(thread_create(func, &id1, (void*)((uint)stack1 + 4096)) < 0){
    printf("thread_create failed\n");
    exit(1);
  }
  if(thread_create(func, &id2, (void*)((uint)stack2 + 4096)) < 0){
    printf("thread_create failed\n");
    exit(1);
  }

  thread_join(0);
  thread_join(0);

  printf("Final: threads joined\n");
  exit(0);
}
