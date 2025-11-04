#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if(argc != 3) {
        printf(2, "Usage: login_test username password\n");
        exit();
    }
    
    int result = authenticate(argv[1], argv[2]);
    if(result >= 0) {
        printf(1, "Authentication successful! UID: %d\n", result);
    } else {
        printf(1, "Authentication failed!\n");
    }
    
    exit();
}