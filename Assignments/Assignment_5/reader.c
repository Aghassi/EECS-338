#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"

void *reader(void *shared_data) {
   printf("%i: entering thread.", gettid());

   // get the shared data
   struct shared_data *shared = (struct shared_data *)shared_data;

   // wait r-mutex
   if(sem_wait(shared->readerMutex) == -1) {
      perror("failed to wait for readerMutex");
      pthread_exit(NULL);
   }

   // wait mutex
   if(sem_wait(shared->mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   // check if we have writers waiting
   if(shared->nWriters > 0) {
      shared->RBlocked = true;

      // signal mutex
      if(sem_post(shared->mutex) == -1) {
         perror("Failed to signal mutex in reader");
         pthread_exit(NULL);
      }

      // wait for reader
      if(sem_wait(shared->sem_reader) == -1) {
         perror("failed to wait for reader in reader");
         pthread_exit(NULL);
      }
   }
   else {
      shared->nReaders++;

      // signal mutex
      if(sem_post(shared->mutex) == -1) {
         perror("Failed to signal mutex in reader");
         pthread_exit(NULL);
      }
   }

   // signal r-mutex
   if(sem_post(shared->readerMutex) == -1) {
      perror("failed to signal for readerMutex");
      pthread_exit(NULL);
   }

   printf("%i: has read!", getpid());

   // wait mutex
   if(sem_wait(shared->mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   shared->nReaders--;
   if(shared->nReaders == 0 && shared->nWriters > 0) {
      // signal mutex for writer
      if(sem_post(shared->sem_writer) == -1) {
         perror("Failed to signal writer in reader");
         pthread_exit(NULL);
      }
   }

   // signal mutex
   if(sem_signal(shared->mutex) == -1) {
      perror("failed to signal mutex in reader");
      pthread_exit(NULL);
   }

   printf("%i: exiting thread.", gettid());

   pthread_exit(NULL);
}
