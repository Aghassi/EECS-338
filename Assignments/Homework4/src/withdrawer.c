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
   // struct sembuf signal_wList = {shared.wList, SIGNAL, 0};  // for signal(wList)

   // Current spot in queue
   int waitNumber = 0;

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

   // Check for wait(mutex)
   printf("checking for mutex \n");
   if(semop(shared.semkey, &wait_mutex, 1) < 0 ) {
      perror("wait(mutex) failed for withdrawer");
      _exit(EXIT_FAILURE);
   }

   // Check if the count is 0 and we can withdraw from given balance
   printf("checking if we can withdraw from the account \n");
   if(mem->wCount == 0 && mem->balance >= amount) {
      mem->balance = mem->balance - amount;

      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("signal(mutex) for withdraw failed");
         _exit(EXIT_FAILURE);
      }
   }
   
   // // We have to wait until we can withdraw
   else {
      printf("We have to wait to withdraw\n");
      // If we have no waiting list, we create one
     //  if(shared.head == NULL) {
     //     shared.head = initLinkedList(amount);
     //  }
     //  else {
     //     addToEndOfList(shared.head, amount);
     // }

      // Increment the count and get the position
      mem->wCount = shared.wCount + 1;
      waitNumber = mem->wCount;

      // signal(mutex)
      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("failed to signal(mutex) after adding to linked list.");
         _exit(EXIT_FAILURE);
      }

      // wait for deposit and that it is our turn in line
      if((semop(shared.semkey, &wait_wList, 1) < 0) && waitNumber == shared.wCount) {
         perror("failed to wait for wList in withdrawer");
         _exit(EXIT_FAILURE);
      }

      /**** Critical Section Start ****/
      // Make withdrawl once we no longer have to wait
      // shared.balance = shared.balance - firstRequestAmount(shared.head);
      mem->balance = mem->balance - amount;

      // deleteFirstRequest(&shared.head);
      mem->wCount = mem->wCount - 1;
      waitNumber = 0;                                 // We no longer need this since we withdrew
      /**** Critical Section End ****/

      // If we still have more things waiting, we signal them
      // if(shared.wCount > 1 && (firstRequestAmount(shared.head) < shared.balance)) {
      //    if(semop(shared.semkey, &signal_wList, 1) < 0) {
      //       perror("failed to signal wList in withdrawer.");
      //       _exit(EXIT_FAILURE);
      //    }
      // }
      // else {
         // This signal is paired with depositing customer's wait(mutex)
         if(semop(shared.semkey, &signal_mutex, 1) < 0) {
            perror("failed to signal mutex for withdrawl");
            _exit(EXIT_FAILURE);
         }
      // }
   }

   printf("The balance is %i \n", mem->balance);
   // Detach from the shared memory
   if(shmdt(mem) < 0) {
      perror("Failed to detatch from shared memory account");
      _exit(EXIT_FAILURE);
   }
   _exit(EXIT_SUCCESS);
}
