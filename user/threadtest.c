#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_THREADS 3

int shared_counter = 0;
lock_t lock;

void worker(void *arg)
{
  int tid = (int)(uint64)arg;
  for (int i = 0; i < 5; i++) {
    lock_acquire(&lock);
    int temp = shared_counter;
    sleep(1);
    shared_counter = temp + 1;
    printf("Thread %d incremented counter to %d\n", tid, shared_counter);
    lock_release(&lock);
  }
  exit(0);
}

int main(void)
{
  printf("Starting thread test...\n");

  lock_init(&lock);

  for (int i = 0; i < NUM_THREADS; i++) {
    void *stack = malloc(4096);
    if (stack == 0) {
      printf("Failed to allocate stack for thread %d\n", i);
      exit(1);
    }

    if (clone(worker, (void *)(uint64)i, stack) < 0) {
      printf("Clone failed for thread %d\n", i);
      exit(1);
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    join(0);
  }

  printf("All threads finished. Final counter = %d\n", shared_counter);
  exit(0);
}
);
}

/*
================= EXPECTED OUTPUT =================

$ threadtest
Starting thread test...
Thread 0 incremented counter to 1
Thread 2 incremented counter to 2
Thread 1 incremented counter to 3
Thread 0 incremented counter to 4
Thread 1 incremented counter to 5
Thread 2 incremented counter to 6
Thread 0 incremented counter to 7
Thread 1 incremented counter to 8
Thread 2 incremented counter to 9
Thread 0 incremented counter to 10
Thread 1 incremented counter to 11
Thread 2 incremented counter to 12
Thread 0 incremented counter to 13
Thread 1 incremented counter to 14
Thread 2 incremented counter to 15
All threads finished. Final counter = 15
==================================================

✅ Explanation:
- Each thread (0, 1, 2) runs the same function using clone().
- Locks prevent race conditions on shared_counter.
- join() ensures the main thread waits for all threads to finish.
- Final counter = NUM_THREADS * 5 = 15.

*/