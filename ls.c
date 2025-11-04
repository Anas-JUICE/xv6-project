#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

// Simple wildcard matching
int match(char *pattern, char *name) {
  if (*pattern == '\0') return *name == '\0';
  if (*pattern == '*') {
    pattern++;
    if (*pattern == '\0') return 1;
    while (*name) {
      if (match(pattern, name)) return 1;
      name++;
    }
    return 0;
  }
  if (*pattern == '?' || *pattern == *name) {
    return *name != '\0' && match(pattern + 1, name + 1);
  }
  return 0;
}

int has_wildcard(char *s) {
  while (*s) {
    if (*s == '*' || *s == '?') return 1;
    s++;
  }
  return 0;
}

char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;
  if(strlen(p) >= DIRSIZ) return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  buf[DIRSIZ] = 0;
  return buf;
}

void ls(char *path) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // If path has wildcards, we need to search in current directory
  char *search_dir = ".";
  char *pattern = path;
  
  if(has_wildcard(path)) {
    search_dir = ".";
  } else {
    search_dir = path;
    pattern = "*"; // match everything
  }

  if((fd = open(search_dir, 0)) < 0){
    printf(2, "ls: cannot open %s\n", search_dir);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", search_dir);
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    close(fd);
    return;
  }

  // Directory listing with pattern matching
  if(strlen(search_dir) + 1 + DIRSIZ + 1 > sizeof buf){
    printf(1, "ls: path too long\n");
    close(fd);
    return;
  }
  
  strcpy(buf, search_dir);
  p = buf+strlen(buf);
  *p++ = '/';
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) continue;
    
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    
    // Check if the filename matches our pattern
    if(!match(pattern, de.name)) continue;
    
    if(stat(buf, &st) < 0){
      printf(1, "ls: cannot stat %s\n", buf);
      continue;
    }
    printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  int i;
  
  if(argc < 2){
    ls(".");
    exit();
  }
  
  for(i=1; i<argc; i++)
    ls(argv[i]);
  
  exit();
}