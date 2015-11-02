#ifndef SEMAPHORE_COMMON
#define SEMAPHORE_COMMON

#include <semaphore.h>

// define boolean type
typedef enum {
   false,
   true
} bool;

// POSIX Semaphores
sem_t mutex, sem_reader, sem_writer;

// Holds information about the shared data
// needed by readers and writers
struct shared_data {
   int nReaders;        // Number of readers
   int nWriters;        // Number of writers
   bool busy;           // busy signal for when it is the writers
   bool RBlocked;       // blocks readers while writers are writing
};

#endif
