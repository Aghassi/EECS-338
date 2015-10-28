#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#include "common.h"


void withdrawer(struct shared_data_info shared, int amount) {
   // Define useful semaphore variables
   struct sembuf wait_mutex = {shared.mutex, WAIT, 0};      // for wait(mutex)
   struct sembuf signal_mutex = {shared.mutex, SIGNAL, 0};  // for signal(mutex)
   struct sembuf wait_wList = {shared.wList, WAIT, 0};      // for wait(wList)
   struct sembuf signal_wList = {shared.wList, SIGNAL, 0};  // for signal(wList)

   // Attach to shared memory
   // since `shmat` returns a pointer to the data,
   // we can treat it however we want
   // here we make it an int since an account has money
   // in it, which we think of as ints
   sharedMemory *mem= shmat(shared.shmid, (void *) 0, 0);
   if(mem < 0) {
      perror("shmat(shared.shmid, (void *) 0, 0)");
      _exit(EXIT_FAILURE);
   }

   printf("The balance before withdrawl is %i \n", mem->balance);

   // Check for wait(mutex)
   if(semop(shared.semkey, &wait_mutex, 1) < 0 ) {
      perror("wait(mutex) failed for withdrawer");
      _exit(EXIT_FAILURE);
   }

   // Check if the count is 0 and we can withdraw from given balance
   if(mem->wCount == 0 && mem->balance >= amount) {
      mem->balance = mem->balance - amount;

      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("signal(mutex) for withdraw failed");
         _exit(EXIT_FAILURE);
      }
   }
   
   // We have to wait until we can withdraw
   else {
      // Increment the count and get the position
      mem->wCount = mem->wCount + 1;

      // signal(mutex)
      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("failed to signal(mutex) after adding to linked list.");
         _exit(EXIT_FAILURE);
      }

      // wait for deposit and that it is our turn in line
      if((semop(shared.semkey, &wait_wList, 1) < 0) && mem->wCount > 0) {
         perror("failed to wait for wList in withdrawer");
         _exit(EXIT_FAILURE);
      }

      /**** Critical Section Start ****/
      // Make withdrawl once we no longer have to wait
      mem->balance = mem->balance - amount;
      mem->wCount = mem->wCount - 1;
      /**** Critical Section End ****/

      printf("Balance decremented, wCount is now %i \n", mem->wCount);
      // If we still have more things waiting, we signal them
      if(shared.wCount > 1 && mem->balance > 0) { 
         if(semop(shared.semkey, &signal_wList, 1) < 0) {
            perror("failed to signal wList in withdrawer.");
            _exit(EXIT_FAILURE);
         }
      }
      else {
         // This signal is paired with depositing customer's wait(mutex)
         if(semop(shared.semkey, &signal_mutex, 1) < 0) {
            perror("failed to signal mutex for withdrawl");
            _exit(EXIT_FAILURE);
         }
      }
   }

   printf("The balance after withdrawl is %i \n", mem->balance);
   // Detach from the shared memory
   if(shmdt(mem) < 0) {
      perror("Failed to detatch from shared memory account");
      _exit(EXIT_FAILURE);
   }
   _exit(EXIT_SUCCESS);
}
