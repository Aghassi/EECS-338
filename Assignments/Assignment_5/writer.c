#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"

void *writer(void *shared_data) {
   printf("%i: entering thread.", gettid());

   // get the shared data
   struct shared_data *shared = (struct shared_data *)shared_data;

   // wait mutex
   if(sem_wait(shared->mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   // Add count to writers
   shared->nWriters++;

   if(shared->busy || shared->nReaders > 0) {
      // signal mutex
      if(sem_post(shared->mutex) == -1) {
         perror("Failed to signal mutex in writer");
         pthread_exit(NULL);
      }

      // wait for writer
      if(sem_wait(shared->sem_writer) == -1) {
         perror("failed to wait for writer in writer");
         pthread_exit(NULL);
      }
   }
   else {
      shared->busy = true;
      // signal mutex
      if(sem_post(shared->mutex) == -1) {
         perror("Failed to signal mutex in writer");
         pthread_exit(NULL);
      }
   }

   printf("%i: has written!", getpid());
   // decrement writers once they write
   shared->nWriters--;
   shared->busy = false;

   if(shared->nWriters > 0) {
      shared->busy = true;

      // signal writer
      if(sem_post(shared->sem_writer) == -1) {
         perror("failed to signal writer in writer");
         pthread_exit(NULL);
      }
   }
   else if (shared->RBlocked){
      shared->RBlocked = false;
      shared->nReaders++;

      // signal reader
      if(sem_post(shared->sem_reader) == -1) {
         perror("failed to signal reader in writer");
         pthread_exit(NULL);
      }
   }

   // signal mutex
   if(sem_post(shared->mutex) == -1) {
       perror("failed to signal mutex in writer");
       pthread_exit(NULL);
    }

   printf("%i: exiting thread.", gettid());

   pthread_exit(NULL);
}
