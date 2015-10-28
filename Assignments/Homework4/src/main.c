#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

#include "common.h"
#include "depositer.h"
#include "withdrawer.h"

void initializeCount(int semkey);
void cleanup(int status);

const int BUF_SIZE = 3;
enum SEMAPHORES {MUTEX = 1, WLIST = 0};

int shmid = -1;
int semkey = -1;
pid_t depositer_id = -1;
pid_t withdrawer_id = -1;

int main() {

      // Create shared memory segment
      // give it read write permissions 0666
      size_t shmsize = BUF_SIZE;
      shmid = shmget(IPC_PRIVATE, shmsize, IPC_CREAT | 0666);
      if(shmid < 0) {
         perror("Error getting shared memory");
         cleanup(EXIT_FAILURE);
      }
      printf("created shared memory \n");

      // Get key to private semaphore group
      // with num_sem semaphores in it
      // create read write permissions if necessary
      semkey = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
      if(semkey < 0) {
         perror("Error getting semaphores");
         cleanup(EXIT_FAILURE);
      }
      initializeCount(semkey);
      printf("created semaphore key \n");

      // We setup the shared memory to be shared by the child processes
      // This will be copied to each child process so they can reference it
      struct shared_data_info shared = {
         .shmid = shmid,
         .semkey = semkey,
         .mutex = MUTEX,
         .wList = WLIST,
         .wCount = 0,
         .balance = 1000
      };

      // Attach to shared memory
      sharedMemory *mem= shmat(shared.shmid, (void *) 0, 0);
      if(mem < 0) {
         perror("shmat(shared.shmid, (void *) 0, 0)");
         _exit(EXIT_FAILURE);
      }

      // Set initial values
      mem->balance = shared.balance;
      mem->wCount = shared.wCount;

      // Detach from the shared memory
      if(shmdt(mem) < 0) {
         perror("Failed to detatch from shared memory for depositer");
         _exit(EXIT_FAILURE);
      }

      int i = 0;
      srand(time(NULL));
      int randAmount = 0;
      for (i = 0; i < 10; i++) {
         randAmount = rand() % 500;
         if(i%2) {
            // Fork withdrawer
            withdrawer_id = fork();
            if(withdrawer_id < 0) {
              perror("Error forking withdrawer");
              cleanup(EXIT_FAILURE);
            }
            else if (!withdrawer_id) {
              withdrawer(shared, randAmount);
            } 
         }
         else {
            // Fork depositer
            depositer_id = fork();
            if(depositer_id < 0) {
               perror("Error forking depositer");
               cleanup(EXIT_FAILURE);
            }
            else if (!depositer_id) {
               sleep(1);
               depositer(shared, randAmount);
            }
         }
         printf("\n");
         sleep(2);  
      }
      // Fork depositer
      // We always make sure to do this so we
      // don't have any lingering withdrawls
      depositer_id = fork();
      if(depositer_id < 0) {
         perror("Error forking depositer");
         cleanup(EXIT_FAILURE);
      }
      else if (!depositer_id) {
         sleep(1);
         depositer(shared, 1000);
      }
      printf("\n");

      

      // Wait for children
      int status1, status2, status3, status4, status5,
       status6, status7, status8, status9, status10,
       status11;
      if(wait(&status1) < 0) {
         perror("wait(&status1)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status2) < 0) {
         perror("wait(&status2)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status3) < 0) {
         perror("wait(&status2)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status4) < 0) {
         perror("wait(&status4)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status5) < 0) {
         perror("wait(&status5)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status6) < 0) {
         perror("wait(&status6)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status7) < 0) {
         perror("wait(&status7)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status8) < 0) {
         perror("wait(&status8)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status9) < 0) {
         perror("wait(&status9)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status10) < 0) {
         perror("wait(&status10)");
         cleanup(EXIT_FAILURE);
      }
      if(wait(&status11) < 0) {
         perror("wait(&status11)");
         cleanup(EXIT_FAILURE);
      }
      int status = WEXITSTATUS(status1) || 
      WEXITSTATUS(status2) || 
      WEXITSTATUS(status3) || 
      WEXITSTATUS(status4) || 
      WEXITSTATUS(status5) || 
      WEXITSTATUS(status6) ||
      WEXITSTATUS(status7) || 
      WEXITSTATUS(status8) || 
      WEXITSTATUS(status9) || 
      WEXITSTATUS(status10)||
      WEXITSTATUS(status11);

      // Mark the children as finished
      withdrawer_id = -1;
      depositer_id = -1;

      cleanup(status);

      // We should never reach this statement
      return status;
}



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
   unsigned short counters[2];
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

/*
 * Make sure that we clean up the remaining processes and
 * we detached from our semaphores properly along with our
 * shared memory group
 */
void cleanup(int status) {
   // Kill children if they're running
   if (depositer_id > 0) {
      if(kill(depositer_id, SIGKILL) < 0) {
         perror("kill(depositer_id, SIGKILL)");
         exit(EXIT_FAILURE);
      }
      wait(NULL);
   }

   if(withdrawer_id > 0) {
      if(kill(withdrawer_id, SIGKILL) < 0) {
         perror("kill(withdrawer_id, SIGKILL)");
         exit(EXIT_FAILURE);
      }
      wait(NULL);
   }

   // For semaphore group with semkey
   // (second argument is ignored)
   // remove it (IPC_RMID)
   if(semkey >= 0) {
      if(semctl(semkey, 0, IPC_RMID) < 0) {
         perror("Error removing semaphores");
         status = EXIT_FAILURE;
      }
   }

   // For shared memory segmnent with id shmid
   // remove it (IPC_RMID)
   // (last argument is ignored, use NULL pointer
   if(shmid >=0) {
      if(shmctl(shmid, IPC_RMID, (struct shmid_ds *) NULL)) {
         perror("Error removing shared memory");
         status = EXIT_FAILURE;
      }
   }

   exit(status);
}
