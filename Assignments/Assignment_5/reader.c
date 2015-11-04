#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"

void *reader(void *shared_data) {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   pid_t tid = syscall(SYS_gettid);
   printf("%i: entering reader thread at time %03ld \n", tid, tv.tv_usec);
   fflush(stdout);

   // get the shared data
   struct shared_data *shared = (struct shared_data *)shared_data;

   // wait mutex
   if(sem_wait(&mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   shared->nReaders++;

   if(shared->nReaders == 1) {
      // wait writer
      if(sem_wait(&sem_writer) == -1) {
         perror("failed to wait for mutex in reader");
         pthread_exit(NULL);
      }
   }
   // signal mutex
   if(sem_post(&mutex) == -1) {
      perror("Failed to signal mutex in reader");
      pthread_exit(NULL);
   }

   /**** Critical Section ****/
   printf("%i: has read! \n", tid);
   fflush(stdout);

   /**** End Critical Section ****/

   // wait mutex
   if(sem_wait(&mutex) == -1) {
      perror("failed to wait for mutex in reader");
      pthread_exit(NULL);
   }

   shared->nReaders--;
   if(shared->nReaders == 0) {
      // signal writer
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

   printf("%i: exiting reader thread. \n", tid);
   printf("\n");
   fflush(stdout);

   pthread_exit(NULL);
}
