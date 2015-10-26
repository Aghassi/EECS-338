#ifndef SEMAPHORE_COMMON
#define SEMAPHORE_COMMON

/*
 * Create 'semenum' structure
 * (copied from semctl man page)
 */
union semenum {
   int val;                // Value for SETVAL
   struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
   unsigned short *array;  // Array for GETALL, SETALL
   struct seminfo *_buf;   // Buffer for IPC_INFO (Linux Specific)
};

typedef struct node {
   int amount;       // Amount waiting withdrawer wishes to deposit
   struct node *next;
} node_t;

// Wait and signal operations
// given to `semop`
#define WAIT -1
#define SIGNAL 1

// Holds information about shared data
// Needed by depositers and withdrawers
struct shared_data_info {
   int BUF_SIZE;  // Size of shared buffer
   int shmid;     // ID for shared memory
   int semkey;    // ID for semaphore group
   int mutex;     // Index for `mutex` semaphore
   int balance;   // The balance in the account
   int wCount;    // Number of withdrawers waiting
   node_t *head;  // Head of the linked list of waiting withdrawers
};

#endif
