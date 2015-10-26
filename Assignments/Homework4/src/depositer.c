#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#include "common.c"
#include "common.h"

void depositer(struct shared_data_info shared, int deposit) {
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
   int *account = shmat(shared.shmid, (void *) 0, 0);
   if(account < 0) {
      perror("shmat(shared.shmid, (void *) 0, 0)");
      _exit(EXIT_FAILURE);
   }

   // Check for wait(mutex)
   if(semop(shared.semkey, &wait_mutex,1) < 0 ) {
      perror("wait(mutex) failed for depositer");
      _exit(EXIT_FAILURE);
   }

   // Deposit money in account
   shared.balance = shared.balance - deposit;

   // If we aren't waiting on any withdrawers, signal mutex
   if(shared.wCount == 0) {
      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("signal(mutex) for deposit failed");
         _exit(EXIT_FAILURE);
      }
   }
   else if(firstRequestAmount(shared.head) > shared.balance) {
      // Otherwise, if the requested amount is greater than the balance we have,
      // we signal the mutex
      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("signal(mutex) for deposit failed");
         _exit(EXIT_FAILURE);
      }
   }
   else {
      // If we do have withdrawers waiting, we signal them
      if(semop(shared.semkey, &signal_wList, 1) < 0) {
         perror("failed to signal wList in withdrawer.");
         _exit(EXIT_FAILURE);
      }
   }

   // Detach from the shared memory
   if(shmdt(account) < 0) {
      perror("Failed to detatch from shared memory for depositer");
      _exit(EXIT_FAILURE);
   }

   _exit(EXIT_SUCCESS);
}
