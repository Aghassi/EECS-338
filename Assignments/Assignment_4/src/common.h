#ifndef SEMAPHORE_COMMON
#define SEMAPHORE_COMMON

/*
 * Create 'semenum' structure
 * (copied from semctl man page)
 */
union semun {
   int val;                // Value for SETVAL
   struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
   unsigned short *array;  // Array for GETALL, SETALL
   struct seminfo *_buf;   // Buffer for IPC_INFO (Linux Specific)
};

#define NUM_PROCESSES 10   // Number of processes which we plan to run

typedef struct sharedMemory {
   int balance;               // The balance in the account
   int wCount;                // The current count of waiting withdrawers
   int orderOfWithdrawls[NUM_PROCESSES/2];  // Static array that houses the waiting withdrawl amounts
} sharedMemory;

// Wait and signal operations
// given to `semop`
#define WAIT -1
#define SIGNAL 1

// Holds information about shared data
// Needed by depositers and withdrawers
struct shared_data_info {
   int shmid;     // ID for shared memory
   int semkey;    // ID for semaphore group
   int mutex;     // Index for `mutex` semaphore
   int balance;   // The starting balance in the account
   int wCount;    // Initial number of withdrawers waiting
   int wList;     // waiting list semeaphore
};

#endif