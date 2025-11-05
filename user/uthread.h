#ifndef UTHREAD_H
#define UTHREAD_H

int clone(void (*fcn)(void*), void *arg, void *stack);
int join(int *stack_ret);

int thread_create(void (*start)(void*), void *arg, void *stack);
int thread_join(int *ret);

#endif
