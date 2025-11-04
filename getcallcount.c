#include "types.h"
#include "user.h"

int main(void) {
    int count = getcallcount();
    printf(1, "Syscall count: %d\n", count);
    exit();
}
