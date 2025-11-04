#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_INPUT 64

void remove_newline(char *str) {
    int len = strlen(str);
    if(len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = 0;
    }
}

int main() {
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    int attempts = 0;
    
    printf(1, "\n=== xv6 Login ===\n");
    
    while(attempts < 3) {
        // Get username
        printf(1, "Username: ");
        memset(username, 0, MAX_INPUT);
        gets(username, MAX_INPUT);
        remove_newline(username);
        
        // Get password  
        printf(1, "Password: ");
        memset(password, 0, MAX_INPUT);
        gets(password, MAX_INPUT);
        remove_newline(password);
        
        printf(1, "Trying: user='%s' pass='%s'\n", username, password);
        
        // Check credentials using system call
        int result = authenticate(username, password);
        
        // FIX: Check for success (any non-negative value)
        if(result >= 0) {
            printf(1, "Login successful! Welcome %s\n", username);
            
            // Launch shell
            char *argv[] = { "sh", 0 };
            exec("sh", argv);
            printf(1, "exec failed!\n");
            exit();
        } else {
            attempts++;
            printf(1, "Login failed. Attempts: %d/3\n", attempts);
        }
    }
    
    printf(1, "Maximum login attempts exceeded.\n");
    exit();
}