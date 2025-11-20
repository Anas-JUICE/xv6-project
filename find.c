#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define MAX_PATH 512

// Implement missing functions
int strncmp(const char *a, const char *b, int n) {
  int i;
  for(i=0; i<n && a[i] && a[i]==b[i]; i++);
  if(i == n) return 0;
  return (uchar)a[i] - (uchar)b[i];
}

char *strrchr(const char *s, char c) {
  char *last = 0;
  while(*s) {
    if(*s == c) last = (char*)s;
    s++;
  }
  return last;
}

void strcat(char *dest, char *src) {
  int i = 0, j = 0;
  while(dest[i]) i++;
  while((dest[i++] = src[j++]));
}

// pattern matching with '*' wildcard
int match_pattern(char *name, char *pat){
  if(strcmp(pat, "*")==0) return 1;
  char *star = 0;
  for(int i=0; pat[i]; i++) if(pat[i]=='*'){ star = &pat[i]; break; }
  if(!star) return strcmp(name, pat)==0;
  int prefix_len = star - pat;
  int suffix_len = 0;
  for(int i=prefix_len+1; pat[i]; i++) suffix_len++;
  int name_len = strlen(name);
  if(name_len < prefix_len + suffix_len) return 0;
  if(prefix_len>0 && strncmp(name, pat, prefix_len)!=0) return 0;
  if(suffix_len>0 && strncmp(name + name_len - suffix_len, star+1, suffix_len)!=0) return 0;
  return 1;
}

void find_rec(char *path, char *pattern){
  char buf[MAX_PATH];
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0) return;
  if(fstat(fd, &st) < 0){ close(fd); return; }

  if(st.type == T_FILE){
    char *slash = strrchr(path, '/');
    char *name = slash ? slash+1 : path;
    if(match_pattern(name, pattern)) printf(1, "%s\n", path);
    close(fd);
    return;
  }

  if(st.type == T_DIR){
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0) continue;
      if(strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0) continue;

      int len = strlen(path);
      if(len + 1 + strlen(de.name) + 1 > sizeof(buf)) continue;

      strcpy(buf, path);
      if(buf[len-1] != '/') strcat(buf, "/");
      strcat(buf, de.name);

      if(stat(buf, &st) < 0) continue;
      if(st.type == T_DIR) find_rec(buf, pattern);
      else if(st.type == T_FILE){
        if(match_pattern(de.name, pattern)) printf(1, "%s\n", buf);
      }
    }
  }

  close(fd);
}

int main(int argc, char *argv[]){
  if(argc != 3){
    printf(2, "usage: find <path> <pattern>\n");
    exit();
  }
  find_rec(argv[1], argv[2]);
  exit();
}
