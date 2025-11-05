/* proc.h -- partial, additions for kernel threads */
#ifndef _PROC_H_
#define _PROC_H_

#include "types.h"
#include "spinlock.h"

struct proc {
  // existing fields are expected to be present in your repo's proc.h
  // Add these fields for thread support:
  int isthread;           // 1 if this proc is a kernel thread sharing address space
  struct proc *tg_master; // thread group master (original process)
  int tg_count;           // valid only in master: number of threads in group
};

#endif
