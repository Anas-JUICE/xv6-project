#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

int
strcmp(const char *s1, const char *s2)
{
    while(*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

#define MAX_USERS 10
#define USERNAME_LEN 32
#define PASSWORD_LEN 32

struct user {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    int uid;
};

struct user users[MAX_USERS] = {
    {"admin", "password", 0},
    {"Anas", "123456", 1},
    {"Anthony", "654321", 2},
    {"", "", -1}  // Terminator
};

// Function to check if credentials are valid
int
check_credentials(char* username, char* password)
{
    struct user *u = users;
    
    // DEBUG: Print what we received
    cprintf("AUTH DEBUG: Received username='", username);
    // Print character by character to see special chars
    for(int i = 0; username[i] != 0; i++) {
        if(username[i] >= 32 && username[i] <= 126) {
            cprintf("%c", username[i]);
        } else {
            cprintf("[0x%x]", username[i] & 0xFF);
        }
    }
    cprintf("' password='");
    for(int i = 0; password[i] != 0; i++) {
        if(password[i] >= 32 && password[i] <= 126) {
            cprintf("%c", password[i]);
        } else {
            cprintf("[0x%x]", password[i] & 0xFF);
        }
    }
    cprintf("'\n");
    
    while(u->uid != -1) {
        cprintf("AUTH DEBUG: Comparing with stored user='%s' pass='%s'\n", u->username, u->password);
        
        int user_match = strcmp(u->username, username);
        int pass_match = strcmp(u->password, password);
        
        cprintf("AUTH DEBUG: user_match=%d, pass_match=%d\n", user_match, pass_match);
        
        if(user_match == 0 && pass_match == 0) {
            cprintf("AUTH DEBUG: SUCCESS for user %s\n", username);
            return u->uid;
        }
        u++;
    }
    cprintf("AUTH DEBUG: FAILED - no match found\n");
    return -1;
}