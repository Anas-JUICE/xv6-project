#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "ticketlock.h"

void
init_ticketlock(struct ticketlock *lk)
{
  lk->next = 0;
  lk->owner = 0;
}

void
ticket_acquire(struct ticketlock *lk)
{
  uint me = xchg(&lk->next, lk->next + 1);
  while(lk->owner != me)
    ;
}

void
ticket_release(struct ticketlock *lk)
{
  lk->owner = lk->owner + 1;
}
