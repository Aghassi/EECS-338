#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#include "common.c"
#include "common.h"

void withdrawer(struct shared_data_info shared, int amount) {
   // Define useful semaphore variables
   struct sembuf wait_mutex = {shared.mutex, WAIT, 0};      // for wait(mutex)
   struct sembuf signal_mutex = {shared.mutex, SIGNAL, 0};  // for signal(mutex)
   struct sembuf wait_full = {shared.full, WAIT, 0};        // for wait(full) when the bank account hits limit
   struct sembuf signal_empty = {shared.empty, SIGNAL, 0};

   // Current spot in queue
   int waitNumber = 0;

   // Attach to shared memory
   // since `shmat` returns a pointer to the data,
   // we can treat it however we want
   // here we make it an int since an account has money
   // in it, which we think of as ints
   int *account = shmat(shared.shmid, (void *) 0, 0);
   if(account < 0) {
      perror("shmat(shared.shmid, (void *) 0, 0)");
      _exit(EXIT_FAILURE);
   }

   // Check for wait(mutex)
   if(semop(shared.semkey, &wait_mutex,1) < 0 ) {
      perror("wait(mutex) failed for withdrawer");
      _exit(EXIT_FAILURE);
   }

   // Check if the count is 0 and we can withdraw from given balance
   if(shared.wCount == 0 && shared.balance > amount) {
      shared.balance = shared.balance - amount;

      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("signal(mutex) for withdraw failed");
         _exit(EXIT_FAILURE);
      }
   }
   // We have to wait until we can withdraw
   else {
      // If we have no waiting list, we create one
      if(shared.head == NULL) {
         shared.head = initLinkedList(amount);
      }
      else {
         addToEndOfList(shared.head, amount);
      }

      // Increment the count and get the position
      shared.wCount = shared.wCount + 1;
      waitNumber = shared.wCount;

      // signal(mutex)
      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("failed to signal(mutex) after adding to linked list.");
         _exit(EXIT_FAILURE);
      }
      // wait for deposit
      //
   }

}
