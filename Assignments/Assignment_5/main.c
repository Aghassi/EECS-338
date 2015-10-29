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

// POSIX Semaphores
sem_t mutex, readerMutex, sem_reader, sem_writer;

// Shared memory
struct shared_data shared;

int main() {
   // Create our threads for readers and writers
   pthread_t readerThread;
   //  pthread_t writerThread;

   // Create thread attirbute and status
   pthread_attr_t attr;
   void *status;

   // Creating a set of attributes to send to the thread
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   // Init semaphore counts
   initCounts();

   // Setting up shared data
   struct shared_data shared = {
      .mutex = &mutex,
      .readerMutex = &readerMutex,
      .sem_reader = &sem_reader,
      .sem_writer = &sem_writer,
      .nReaders = 0,
      .nWriters = 0,
      .busy = false,
      .RBlocked = false
   };

   // Fork reader
   if(pthread_create(&readerThread, &attr, reader, (void *)&shared) == -1) {
         perror("pthread_create(readerThread)");
         exit(EXIT_FAILURE);
   }

   // Wait for children
   if(pthread_join(readerThread, &status) == -1) {
      perror("pthread_join(readerThread)");
      exit(EXIT_FAILURE);
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
   if(sem_init(&readerMutex, 0, 0) < 0) {
      perror("sem_init(&readerMutex)");
      exit(EXIT_FAILURE);
   }
   if(sem_init(&sem_reader, 0, 0) < 0) {
      perror("sem_init(&reader)");
      exit(EXIT_FAILURE);
   }
   if(sem_init(&sem_writer, 0, 0) < 0) {
      perror("sem_init(&writer)");
      exit(EXIT_FAILURE);
   }
}
