#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"

void *writer(void *shared_data) {
   pid_t tid = syscall(SYS_gettid);
   printf("%i: entering thread. \n", tid);

   // get the shared data
   struct shared_data *shared = (struct shared_data *)shared_data;

   // wait mutex
   if(sem_wait(&mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   // Add count to writers
   shared->nWriters++;

   if(shared->nReaders > 0) {
      // signal mutex
      if(sem_post(&mutex) == -1) {
         perror("Failed to signal mutex in writer");
         pthread_exit(NULL);
      }

      // wait for writer
      if(sem_wait(&sem_writer) == -1) {
         perror("failed to wait for writer in writer");
         pthread_exit(NULL);
      }
   }

   printf("%i: has written! \n", tid);

   // decrement writers once they write
   shared->nWriters--;
   if(shared->nWriters > 0) {
      shared->busy = true;

      // signal writer
      if(sem_post(&sem_writer) == -1) {
         perror("failed to signal writer in writer");
         pthread_exit(NULL);
      }
   }

   // signal mutex
   if(sem_post(&mutex) == -1) {
       perror("failed to signal mutex in writer");
       pthread_exit(NULL);
    }

   printf("%i: exiting thread. \n", tid);

   pthread_exit(NULL);
}
