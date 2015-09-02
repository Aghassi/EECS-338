#include <stdlib.h>
#include <stdio.h>
#include <processes.h>

int main(int argc, char const *argv[])
{
    // Define a fork count so we can iterate
    int forkCount = 4;
    int loopCount = 1

    for (loopCount; loopCount < forkCount; loopCount++) {
        switch(loopCount) {
            case loopCount == 1:
                runProcess1();
            case loopCount == 2:
                runProcess2();
            case loopCount == 3:
                runProcess3();
            case loopCount == 4:
                runProcess4();
        }
    }

    wait();
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