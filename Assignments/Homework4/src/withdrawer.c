#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#include "common.h"

void withdrawer(struct shared_data_info shared) {
   // Define useful semaphore variables
   struct sembuf wait_mutex = {shared.mutex, WAIT, 0};      // for wait(mutex)
   struct sembuf signal_mutex = {shared.mutex, SIGNAL, 0};  // for signal(mutex)
   struct sembuf wait_full = {shared.full, WAIT, 0};        // for wait(full) when the bank account hits limit
   struct sembuf signal_empty = {shared.empty, SIGNAL, 0};

   // Attach to shared memory
   // since `shmat` returns a pointer to the data,
   // we can treat it however we want
   // here we make it an int since an account has money
   // in it, which we think of as ints
   int *account_buf = shmat(shared.shmid, (void *) 0, 0);
   if(account_buf < 0) {
      perror("shmat(shared.shmid, (void *) 0, 0)");
      _exit(EXIT_FAILURE);
   }

}
