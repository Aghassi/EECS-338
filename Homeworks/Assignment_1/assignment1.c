#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "processes.c"
#include "helper.c"

#define NUM_FORKS 4

int main(int argc, char const *argv[])
{
    pid_t pid;
    int loopCount = 0;
    int status;

    // char * cuser_id_str;

    // cuser_id_str = (char *) cuserid(NULL);

    // printf("Parent cuserid: %s \n", cuser_id_str);
    printIDs("Parent");

    for (loopCount; loopCount < NUM_FORKS; loopCount++) {
        pid = fork();

        if(pid == 0) {
            // We are a child
            switch (loopCount) {
                case 0:
                    runProcess1();
                    return 0;
                case 1:
                    sleep(1);
                    runProcess2();
                    return 0;
                case 2:
                    sleep(2);
                    runProcess3();
                    return 0;
                case 3:
                    sleep(10);
                    runProcess4();
            }
        }
        else if(pid < 0) {
            perror("PID failed.");
        }
    }

    // Parent
    if(pid > 0) {
        int i = 0;
        for (i; i < NUM_FORKS; i++)
        {
            // Get a pointer to status so wait can return it.
            pid = wait(&status);
            printf("Process %d exited with status %d \n", pid, WEXITSTATUS(status));
        }
    }
    /*
    Loop over all the processes so we know when all the child processes are over
    On each loop we can iterate / check the fork count to see what child we are dealing with
    Move child methods to separate file for neatness

    On each loop call method for child process
    At beginning of method call check against child processes fork count so we know 
    whether or not the code should be executed

    USE EXECVP since exec isn't really a thing anymore.
    */
    return 0;
}