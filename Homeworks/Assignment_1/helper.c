#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
Prints the uid, euid, gid, and egid of the current process
@param name a char array with the name of the process
**/
void printIDs(char *name) {
    //Declare variables
    uid_t euid, uid;
    gid_t gid, egid;

    //Populate them
    uid = getuid();
    euid = geteuid();
    gid = getgid();
    egid = getegid();
    
    //Print thems
    printf("%s uid: %d \n", name, uid);
    printf("%s euid: %d \n", name, euid);
    printf("%s gid: %d \n", name, gid);
    printf("%s egid: %d \n", name, egid);
}
