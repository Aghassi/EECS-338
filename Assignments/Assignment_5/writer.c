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

void *writer(void *shared_data) {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   pid_t tid = syscall(SYS_gettid);
   fflush(stdout);
   printf("%i: entering writer thread at time %03ld \n", tid, tv.tv_usec);
   fflush(stdout);

   int i;
   for (i = 0; i < 100000; i++)
   {
     /* no-op */
   }

   // wait writer
   if(sem_wait(&sem_writer) == -1) {
      perror("failed to wait for writer in reader");
      pthread_exit(NULL);
   }

   /**** Critical Section ****/
   for (i = 0; i < 10000; i++)
   {
      /* no-op */
   }
   fflush(stdout);
   printf("%i: has written! \n", tid);
   fflush(stdout);
   /**** End Critical Section ****/

   // signal writer
   if(sem_post(&sem_writer) == -1) {
       perror("failed to signal writer in writer");
       pthread_exit(NULL);
    }

    for (i = 0; i < 110000; i++)
    {
       /* no-op */
    }

   gettimeofday(&tv, NULL);
   fflush(stdout);
   printf("%i: exiting writer thread at time %03ld \n", tid, tv.tv_usec);
   printf("\n");
   fflush(stdout);

   pthread_exit(NULL);
}
