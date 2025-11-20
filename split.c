#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUF 512

void
usage(){
  printf(2, "usage: split <file> <size-in-bytes>\n");
  exit();
}

// convert integer to string (xv6 safe)
void
itoa(int n, char *buf){
  int i = 0, sign = n;
  if(sign < 0) n = -n;
  do {
    buf[i++] = n % 10 + '0';
  } while((n /= 10) > 0);
  if(sign < 0) buf[i++] = '-';
  buf[i] = '\0';

  // reverse
  for(int j = 0, k = i-1; j<k; j++, k--){
    char tmp = buf[j];
    buf[j] = buf[k];
    buf[k] = tmp;
  }
}

void
make_outname(char *base, int part, char *out)
{
  char tmp[16];
  strcpy(out, base);
  int len = strlen(out);
  out[len] = '.';
  out[len+1] = '\0';

  itoa(part, tmp);

  // manual append tmp to out
  int i = 0;
  while(tmp[i] != '\0'){
    out[len+1+i] = tmp[i];
    i++;
  }
  out[len+1+i] = '\0';
}


int
main(int argc, char *argv[]){
  if(argc != 3) usage();

  char *path = argv[1];
  int chunk = atoi(argv[2]);
  if(chunk <= 0){
    printf(2, "split: invalid size\n");
    exit();
  }

  int fd = open(path, O_RDONLY);
  if(fd < 0){
    printf(2, "split: cannot open %s\n", path);
    exit();
  }

  char buf[BUF];
  int part = 0;
  char outname[256];

  while(1){
    int created = 0;
    int remaining = chunk;

    make_outname(path, part++, outname);

    int ofd = open(outname, O_CREATE | O_WRONLY);
    if(ofd < 0){
      printf(2, "split: cannot create %s\n", outname);
      close(fd);
      exit();
    }

    while(remaining > 0){
      int r = read(fd, buf, (remaining > BUF) ? BUF : remaining);
      if(r <= 0) break;
      write(ofd, buf, r);
      remaining -= r;
      created = 1;
    }

    close(ofd);

    if(!created){
      unlink(outname);
      break;
    }

    if(remaining > 0) break;
  }

  close(fd);
  exit();
}
