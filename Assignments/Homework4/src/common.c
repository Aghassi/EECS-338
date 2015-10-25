#include <stdio.h>
#include "common.h"

typedef struct node {
   int amount;       // Amount waiting withdrawer wishes to deposit
   struct node *next;
} node_t;

/*
 * Initialize a linked list to house the waiting values to be withdrawn
 *
 * @param initialAmount the amount to be the first item in the list
 * @return node_t A pointer to the head of the list
 */
(node_t *) initLinkedList(int initialAmount) {
   node_t *head = NULL;
   head = malloc(sizeof(node_t));

   if (head == NULL) {
      perror("head failed to initialize for linked list");
      _exit(EXIT_FAILURE);
   }

   head->amounnt = initialAmount;
   head->next = NULL;

   return head;
}

/*
 * Adds the newest withdrawer amount to the end of the list
 *
 * @param head A pointer to the head of the list
 * @param amountToAdd The amount from the withdrawer to add to the list
 */
void addToEndOfList(node_t *head, int amountToAdd) {
   node_t *current = head;
   node_t *next = head->next;

   while (next != NULL) {
      current = next;
      next = current->next;
   }

   current->next = malloc(sizeof(node_t));
   current->next->val = amountToAdd;
   current->next->next = NULL;

   println("Added %i to end of list", amountToAdd);
}

/*
 * Removes the first item in the linked list
 *
 * @param *head The head of the list to be "popped"/removed
 */
void deleteFirstRequest(node_t **head) {
   int retval = -1;
   node_t * next_node = NULL;

   if(*head == NULL) {
      perror("No head to delete from");
      _exit(EXIT_FAILURE);
   }

   next_node = (*head)->next;
   retval = (*head)->val;
   free(*head);
   *head = next_node;

   println("removed amount %i from the from of the list", retval);
}
