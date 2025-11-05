#ifndef TICKETLOCK_H
#define TICKETLOCK_H

struct ticketlock {
  uint next;
  uint owner;
};

void init_ticketlock(struct ticketlock *lk);
void ticket_acquire(struct ticketlock *lk);
void ticket_release(struct ticketlock *lk);

#endif
