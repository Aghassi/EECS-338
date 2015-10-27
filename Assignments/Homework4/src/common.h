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

typedef struct node {
   int amount;       // Amount waiting withdrawer wishes to deposit
   struct node *next;
} node_t;

typedef struct sharedMemory {
   int balance;
   int wCount;
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
   int balance;   // The balance in the account
   int wCount;    // Number of withdrawers waiting
   int wList;     // List of waiting customers who wish to withdraw
   node_t *head;  // Head of the linked list of waiting withdrawers
};

node_t* initLinkedList(int initialAmount);

void addToEndOfList(node_t *head, int amountToAdd);

void deleteFirstRequest(struct node **head);

int firstRequestAmount(struct node *head);


#endif
