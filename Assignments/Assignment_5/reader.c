#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"

void *reader(void *shared_data) {
   pid_t tid = syscall(SYS_gettid);
   printf("%i: entering thread. \n", tid);

   // get the shared data
   struct shared_data *shared = (struct shared_data *)shared_data;

   // wait mutex
   if(sem_wait(&mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   // check if we have writers waiting
   if(shared->nReaders > 0) {

      // signal mutex
      if(sem_post(&mutex) == -1) {
         perror("Failed to signal mutex in reader");
         pthread_exit(NULL);
      }
   }
   else {
      shared->nReaders++;

      // signal mutex
      if(sem_post(&mutex) == -1) {
         perror("Failed to signal mutex in reader");
         pthread_exit(NULL);
      }
   }

   printf("%i: has read! \n", tid);

   // wait mutex
   if(sem_wait(&mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   shared->nReaders--;
   if(shared->nReaders == 0 && shared->nWriters > 0) {
      // signal mutex for writer
      if(sem_post(&sem_writer) == -1) {
         perror("Failed to signal writer in reader");
         pthread_exit(NULL);
      }
   }

   // signal mutex
   if(sem_post(&mutex) == -1) {
      perror("failed to signal mutex in reader");
      pthread_exit(NULL);
   }

   printf("%i: exiting thread. \n", tid);

   pthread_exit(NULL);
}
