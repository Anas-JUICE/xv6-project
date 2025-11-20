// user/schedtest.c
#include "types.h"
#include "stat.h"
#include "user.h"

int
busy(int seconds){
  int end = uptime() + seconds*100; // uptime in ticks (if tick=10ms) adjust as appropriate
  volatile int x=0;
  while(uptime() < end){
    x++;
  }
  return x;
}

int
main(int argc, char *argv[]){
  int i;
  for(i=0;i<6;i++){
    if(fork() == 0){
      // child: busy loop for few seconds to consume CPU
      printf(1, "child %d pid %d starting\n", i, getpid());
      busy(2 + i%3);
      printf(1, "child %d pid %d done\n", i, getpid());
      exit();
    }
  }
  for(i=0;i<6;i++) wait();
  exit();
}
