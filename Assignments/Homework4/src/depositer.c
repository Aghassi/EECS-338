#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#include "common.h"

void depositer(struct shared_data_info shared, int deposit) {
   // Define useful semaphore variables
   struct sembuf wait_mutex = {shared.mutex, WAIT, 0};      // for wait(mutex)
   struct sembuf signal_mutex = {shared.mutex, SIGNAL, 0};  // for signal(mutex)
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

   printf("Shared memory balance is %i \n", mem->balance);

   // Check for wait(mutex)
   if(semop(shared.semkey, &wait_mutex, 1) < 0 ) {
      perror("wait(mutex) failed for depositer");
      _exit(EXIT_FAILURE);
   }

  printf("Balance is: %i \n", mem->balance);

   // Deposit money in account
   mem->balance = mem->balance + deposit;

   // // If we aren't waiting on any withdrawers, signal mutex
   if(mem->wCount == 0) {
      if(semop(shared.semkey, &signal_mutex, 1) < 0) {
         perror("signal(mutex) for deposit failed");
         _exit(EXIT_FAILURE);
      }
   }
   // else if(firstRequestAmount(shared.head) > shared.balance) {
   //    // Otherwise, if the requested amount is greater than the balance we have,
   //    // we signal the mutex
   //    if(semop(shared.semkey, &signal_mutex, 1) < 0) {
   //       perror("signal(mutex) for deposit failed");
   //       _exit(EXIT_FAILURE);
   //    }
   // }
   else {
      // If we do have withdrawers waiting, we signal them
      printf("We signal wList \n");
      if(semop(shared.semkey, &signal_wList, 1) < 0) {
         perror("failed to signal wList in withdrawer.");
         _exit(EXIT_FAILURE);
      }
   }

   printf("Balance is: %i \n", mem->balance);

   // Detach from the shared memory
   if(shmdt(mem) < 0) {
      perror("Failed to detatch from shared memory for depositer");
      _exit(EXIT_FAILURE);
   }

   _exit(EXIT_SUCCESS);
}
