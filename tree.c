#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void tree(char *path, int depth) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        printf(2, "tree: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        printf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        printf(1, "%s\n", path);
        close(fd);
        return;
    }

    if (depth == 0)
        printf(1, ".\n");

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (stat(buf, &st) < 0) {
            printf(1, "tree: cannot stat %s\n", buf);
            continue;
        }

        for (int i = 0; i < depth; i++)
            printf(1, "    ");
        printf(1, "|-- %s\n", de.name);

        if (st.type == T_DIR) {
            tree(buf, depth + 1);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf(2, "Usage: tree [dirname]\n");
        exit();
    }

    if (argc == 2) {
        tree(argv[1], 0);
    } else {
        tree(".", 0);
    }
    exit();
}