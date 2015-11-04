#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "reader.h"
#include "writer.h"
#include "common.h"

void initCounts();


// Shared memory
struct shared_data shared;

int main() {
   // Create our threads for readers and writers
   pthread_t threads[NUM_THREADS];

   // status variable for returning how program ran
   void *status;

   // Init semaphore counts
   initCounts();

   // Setting up shared data
   struct shared_data shared = {
      .nReaders = 0
   };

   int i;
   for (i = 0; i < NUM_THREADS; i++)
   {
      fflush(stdout);
      if (i%2 == 0) {
         // Fork writer
         if(pthread_create(&threads[i], NULL, writer, (void *)&shared) == -1) {
               perror("pthread_create(writerThread)");
               exit(EXIT_FAILURE);
         }
      }
      else {
         // Fork reader
         if(pthread_create(&threads[i], NULL, reader, (void *)&shared) == -1) {
               perror("pthread_create(readerThread)");
               exit(EXIT_FAILURE);
         }
      }
      fflush(stdout);
   }  

   for (i = 0; i < NUM_THREADS; i++)
   {
      // Wait for children
      if(pthread_join(threads[i], &status) == -1) {
         perror("pthread_join(thread)");
         exit(EXIT_FAILURE);
      }
   }

   return EXIT_SUCCESS;
}

// POSIX semaphore initilization
// uses call sem_init
void initCounts() {
   if(sem_init(&mutex, 0, 1) < 0) {
      perror("sem_init(&mutex)");
      exit(EXIT_FAILURE);
   }
   if(sem_init(&sem_writer, 0, 1) < 0) {
      perror("sem_init(&writer)");
      exit(EXIT_FAILURE);
   }
   printf("All semaphores initialized\n");
   printf("\n");
}
