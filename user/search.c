
// cat > test.txt
// hello world another hello here no match here
// CTRL  D
// search hello test.txt


#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUF_SIZE 512

void
usage(){
  printf(2, "usage: search <keyword> <file>\n");
  exit();
}
int contains(char *line, char *keyword) {
  char *p1, *p2;
  for (; *line; line++) {
    p1 = line;
    p2 = keyword;
    while (*p1 && *p2 && *p1 == *p2) {
      p1++;
      p2++;
    }
    if (*p2 == 0)
      return 1; // match found
  }
  return 0; // no match
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    usage();
  }
  char *keyword = argv[1];
  char *path = argv[2];
  int fd = open(path, O_RDONLY);
  if(fd < 0){
    printf(2, "search: cannot open %s\n", path);
    exit();
  }
  char buf[BUF_SIZE+1];
  int n;
  int line_no = 1;
  char line[1024];
  int line_pos = 0;
  while((n = read(fd, buf, BUF_SIZE)) > 0){
    for(int i=0;i<n;i++){
      char c = buf[i];
      if(c == '\n' || line_pos >= 1023){
        line[line_pos] = 0;
        if(contains(line, keyword)){
          printf(1, "%s:%d: %s\n", path, line_no, line);
        }
        line_pos = 0;
        line_no++;
      } else {
        line[line_pos++] = c;
      }
    }
  }
  // final line
  if(line_pos>0){
    line[line_pos] = 0;
    if(contains(line, keyword)){
      printf(1, "%s:%d: %s\n", path, line_no, line);
    }
  }
  close(fd);
  exit();
}
