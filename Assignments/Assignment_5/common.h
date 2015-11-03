#ifndef SEMAPHORE_COMMON
#define SEMAPHORE_COMMON

#include <semaphore.h>

// POSIX Semaphores
sem_t mutex, sem_reader, sem_writer;

// Holds information about the shared data
// needed by readers and writers
struct shared_data {
   int nReaders;        // Number of readers
};

#endif
