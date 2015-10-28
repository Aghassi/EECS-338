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

const int BUF_SIZE = 3;                // Size of shared memory in bytes
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

      // Attach to shared memory so we can set initial values
      sharedMemory *mem= shmat(shared.shmid, (void *) 0, 0);
      if(mem < 0) {
         perror("shmat(shared.shmid, (void *) 0, 0)");
         _exit(EXIT_FAILURE);
      }

      // Set initial values
      mem->balance = shared.balance;
      mem->wCount = shared.wCount;

      // Detach from the shared memory once initial values are set
      if(shmdt(mem) < 0) {
         perror("Failed to detatch from shared memory for depositer");
         _exit(EXIT_FAILURE);
      }

      // Initialize a count
      // setup the random integer generator
      // initialize random variable
      int i = 0;
      srand(time(NULL));
      int randAmount = 0;

      // Create a random number for each child process
      // if the process is an even integer, we created a withdrawer
      // if the process is odd, we create a depositer
      for (i = 0; i < NUM_PROCESSES; i++) {
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
         // This is here to make output more legible
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
      // Waits on NUM_PROCESSES + 1 (since we always
      // have one more depositer)
      // Will report status
      int statuses[NUM_PROCESSES + 1];
      int n, sti;
      for (n = 0; n < NUM_PROCESSES + 1; n++) {
         if(wait(&statuses[n]) < 0) {
            perror("wait(&status[n])");
            cleanup(EXIT_FAILURE);
         }   
      }
      int status;
      for (sti = 0; sti < NUM_PROCESSES + 1; sti++) {
         status = status || WEXITSTATUS(statuses[sti]);
      }

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