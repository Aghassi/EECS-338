#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

#define LOOP_ITERATIONS (0x7fffffff) // biggest positive 32 bit integer

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

/**
Prints the user and system times for the given process
@param the time to print
@param process name as a char
**/

void printUserAndSystemTime(char *process) {
    struct tms process_times;
    clock_t result;
    double utime_seconds;
    double stime_seconds;

    result = times(&process_times);
    if (result == (clock_t) -1) {
        perror("times() failed \n");
        exit(EXIT_FAILURE);
    }

    utime_seconds = (double) process_times.tms_utime / CLOCKS_PER_SEC;
    stime_seconds = (double) process_times.tms_utime / CLOCKS_PER_SEC;

    printf("User time for %s: %f \n", process, utime_seconds);
    printf("System time for %s: %f \n", process, stime_seconds);
}