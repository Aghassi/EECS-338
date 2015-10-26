#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

#include "common.h"
#include "depositer.h"
#include "withdrawer.h"

void initializeCount(int semkey);
void cleanup(int status);

enum SEMAPHORES {MUTEX = 0, WLIST = 0, NUM_SEM};

/** Body of Main**/



/** Helper methods **/

/*
 * Since semaphores are located sequentially in memory in System V
 * we need to make each group accessable via an array
 * We use the enum array above to access the semaphore, even if only one is needed
 *
 * @param semkey Private key used to access and generate semaphores
 */
void initializeCount(int semkey) {
   // Create uniont structure for counts
   union semun sem_union;
   unsigned short counters[2]
   counters[MUTEX] = 1;
   counters[WLIST] = 0;
   sem_union.array = counters;

   // call semctl to set all counts
   // (second argument is ignored)
   int semset = semctl(semkey, 0, SETALL, sem_union);
   if(semset < 0) {
      perror("Error setting semaphore counts");
      exit(EXIT_FAILURE);
   }
}

void cleanup(int status) {
   // Kill children if they're running
   if (depositer_id > 0) {
      if(kill(depositer_id, SIGKILL) < 0) {
         perror("kill(depositer_id, SIGKILL)");
         exit(EXIT_FAILURE);
      }
      wait(NULL);
   }

}
