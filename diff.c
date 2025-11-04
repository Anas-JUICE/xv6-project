#include "types.h"
#include "stat.h"
#include "user.h"

#define MAXLINE 512

int readline(int fd, char *buf, int max) {
    int i = 0;
    char c;
    while (i < max - 1) {
        if (read(fd, &c, 1) != 1) break;
        if (c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return i;
}

int main(int argc, char *argv[]) {
    int fd1, fd2;
    char line1[MAXLINE], line2[MAXLINE];
    int len1, len2;
    int line_num = 1;
    int differ = 0;

    if (argc != 3) {
        printf(2, "Usage: diff file1 file2\n");
        exit();
    }

    if ((fd1 = open(argv[1], 0)) < 0) {
        printf(2, "diff: cannot open %s\n", argv[1]);
        exit();
    }

    if ((fd2 = open(argv[2], 0)) < 0) {
        printf(2, "diff: cannot open %s\n", argv[2]);
        close(fd1);
        exit();
    }

    while (1) {
        len1 = readline(fd1, line1, MAXLINE);
        len2 = readline(fd2, line2, MAXLINE);

        if (len1 == 0 && len2 == 0) break;

        if (strcmp(line1, line2) != 0) {
            printf(1, "%dc%d\n", line_num, line_num);
            printf(1, "< %s\n", line1);
            printf(1, "---\n");
            printf(1, "> %s\n", line2);
            differ = 1;
        }
        line_num++;
    }

    close(fd1);
    close(fd2);

    if (!differ) printf(1, "Files are identical\n");

    exit();
}