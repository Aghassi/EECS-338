#ifndef SEMAPHORE_COMMON
#define SEMAPHORE_COMMON

#include <semaphore.h>

// define boolean type
typedef enum {
   false,
   true
} bool;

// Holds information about the shared data
// needed by readers and writers
struct shared_data {
   sem_t *mutex;        // `mutex` semaphore
   sem_t *readerMutex;  // `mutex` for readers semaphore
   sem_t *sem_writer;       // `writer` semaphore
   sem_t *sem_reader;       // `reader` semaphore
   int nReaders;        // Number of readers
   int nWriters;        // Number of writers
   bool busy;           // busy signal for when it is the writers
   bool RBlocked;       // blocks readers while writers are writing
};

#endif
