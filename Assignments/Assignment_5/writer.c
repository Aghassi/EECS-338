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
   printf("%i: entering writer thread. \n", tid);
   fflush(stdout);


   // wait writer
   if(sem_wait(&sem_writer) == -1) {
      perror("failed to wait for writer in reader");
      pthread_exit(NULL);
   }

   /**** Critical Section ****/
   printf("%i: has written! \n", tid);
   fflush(stdout);
   /**** End Critical Section ****/

   // signal writer
   if(sem_post(&sem_writer) == -1) {
       perror("failed to signal writer in writer");
       pthread_exit(NULL);
    }

   printf("%i: exiting writer thread. \n", tid);
   printf("\n");
   fflush(stdout);

   pthread_exit(NULL);
}
