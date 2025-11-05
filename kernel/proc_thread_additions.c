/* Additions for proc.c: clone and join implementations.
   This file is intended to be merged into kernel/proc.c or compiled and linked
   with the rest of the kernel sources depending on build system.
*/

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

extern struct proc proc[NPROC];
extern struct spinlock ptable_lock;

int
clone(void (*fcn)(void *), void *arg, void *stack)
{
  struct proc *np;
  struct proc *p = myproc();

  if(stack == 0 || ((uint)stack % 4) != 0)
    return -1;

  acquire(&ptable_lock);
  // allocate proc
  for(np = proc; np < &proc[NPROC]; np++){
    if(np->state == UNUSED){
      // found
      break;
    }
  }
  if(np >= &proc[NPROC]){
    release(&ptable_lock);
    return -1;
  }
  // initialize proc (mimic allocproc minimal behavior)
  np->state = EMBRYO;
  np->pid = nextpid++; // nextpid declared elsewhere in your proc.c; if not, use allocproc
  np->isthread = 1;
  np->tg_master = p->tg_master ? p->tg_master : p;
  if(np->tg_master == p) np->tg_master->tg_count = (np->tg_master->tg_count)+1;
  // share address space
  np->pgdir = p->pgdir;
  np->sz = p->sz;
  // copy trapframe
  *(np->tf) = *(p->tf);

  // set up user stack and start function
  // place argument on stack
  uint sp = (uint)stack;
  sp -= 4;
  if(copyout(np->pgdir, sp, (char*)&arg, 4) < 0){
    np->state = UNUSED;
    release(&ptable_lock);
    return -1;
  }
  np->tf->esp = sp;
  np->tf->eip = (uint)fcn;
  np->tf->eax = 0; // child returns 0

  np->state = RUNNABLE;
  release(&ptable_lock);
  return np->pid;
}

int
join(int *stack_ret)
{
  struct proc *p = myproc();
  struct proc *t;
  int havekids;
  int pid;

  acquire(&ptable_lock);
  for(;;){
    havekids = 0;
    for(t = proc; t < &proc[NPROC]; t++){
      if(t->tg_master != p && t->tg_master != p->tg_master)
        continue;
      if(t->tg_master == p->tg_master && t->isthread){
        havekids = 1;
        if(t->state == ZOMBIE){
          // found a zombie thread in our thread group
          pid = t->pid;
          if(stack_ret && copyout(p->pgdir, (uint)stack_ret, (char*)&t->tf->esp, 4) < 0){
            release(&ptable_lock);
            return -1;
          }
          // free process structure (minimal)
          t->state = UNUSED;
          t->pid = 0;
          release(&ptable_lock);
          return pid;
        }
      }
    }
    if(!havekids){
      release(&ptable_lock);
      return -1;
    }
    sleep(p, &ptable_lock); // wait for a child thread to exit
  }
}
