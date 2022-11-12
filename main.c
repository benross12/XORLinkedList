#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct node {
  char* name;
	struct node* xor_value;
} Node;
Node* calculate_xor_value(Node* before, Node* after) {
  return (Node*)((__intptr_t)before ^ (__intptr_t)after);
}

// returns the address of the next Node, given the current node and the previous one
// the address of next = address of previous XOR current npx (xor_value)
Node* getNextAddress(Node* previous, Node* current) {
  return (Node*)((__intptr_t)previous ^ (__intptr_t)(current -> xor_value));
}

// print the list in order
void printList(Node *head) {
  // get the first two nodes
  Node *current = head;
  Node *before = NULL;
  int count = 0;

  printf("Contents of list \n");
  // traverse the list until we find the target string
  while (current != NULL) {
    Node *next = getNextAddress(before, current);

    printf("%d - %s\n", count, current -> name);
    before = current;
    current = next;
    count++;
  }
  printf("-------------------------\n");
}

// insert a new head at the start of the list
void insert_string(Node** head, const char* newObj) {
  // allocate memory for a node on the heap
  Node *n = (Node*) malloc((sizeof(Node)));
  if (!n) return;

  // copy the value into the node
  n -> name = (char *) newObj;

  // npx of first node in list will always be the address of the next node
  n -> xor_value = *head;

  // change the npx of the next node to include the new head
  if (*head != NULL) {
    (*head) -> xor_value = calculate_xor_value(n, (*head) -> xor_value);
  }
  
  // set the head of the list as the new node
  *head = n;
}

// If possible, inserts before the string "before" and returns true. Returns false if not possible (e.g., the before string is not in the list).
int insert_before(Node** head, const char* before, const char* newObj) {
  // get the first two nodes
  Node *current = *head;
  Node *beforeNode = NULL;

  // traverse the list until we find the target string
  while (current != NULL) {
    // check if the current node is "before"
    if (strcmp(current -> name, before) == 0) {
      Node *next = getNextAddress(beforeNode, current);
      
      // allocate a new node
      Node *newNode = (Node*) malloc(sizeof(Node));
      if (!newNode) return 0;
      newNode -> name = (char *) newObj;
      newNode -> xor_value = calculate_xor_value(beforeNode, current);

      // change the npx of current node
      current -> xor_value = calculate_xor_value(newNode, next);
      
      // change the npx of the previous node
      if (beforeNode) {
        Node *beforeBefore = getNextAddress(current, beforeNode);
        beforeNode -> xor_value = calculate_xor_value(newNode, beforeBefore);
      }
      else { // if there is no previous node, the new node is the new head
        *head = newNode;
      }

      // return successful
      return 1;
    }

    // move on to the next node
    Node *next = getNextAddress(beforeNode, current);
    beforeNode = current;
    current = next;
  }

  // if element not found, return unsuccessful
  return 0;
}

// If possible, inserts after the string "after" and returns true. Returns false if not possible (e.g., the after string is not in the list).
int insert_after(Node** head, const char* after, const char* newObj) {
  // get the first two nodes
  Node *current = *head;
  Node *before = NULL;

  // traverse the list until we find the target string
  while (current != NULL) {    
    // check if the current node is "after"
    if (strcmp(current -> name, after) == 0) {
      Node *next = getNextAddress(before, current);
      
      // allocate memory for the new node
      Node *newNode = (Node*) malloc(sizeof(Node));
      if (!newNode) return 0;
      newNode -> name = (char *) newObj;
      newNode -> xor_value = calculate_xor_value(current, next);
      
      // change the npx of current node
      current -> xor_value = calculate_xor_value(before, newNode);
      
      // change the npx of next node
      if (next) {
        Node *nextNext = getNextAddress(current, next);
        next -> xor_value = calculate_xor_value(newNode, nextNext);
      }

      // return successful
      return 1;
    }

    // move on to the next node
    Node *next = getNextAddress(before, current);
    before = current;
    current = next;
  }

  // return unsuccessful
  return 0;
}

// If possible removes the string at the beginning of the XOR Linked list and returns its value in result. If successful return true, otherwise returns false
int remove_string(Node** head, char* result) {
  // if the list is empty, return false
  if (*head == NULL) return 0;

  // set the result value
  strcpy(result, (*head) -> name);

  // make the second node the new head
  Node *originalHead = *head;
  *head = (*head) -> xor_value;
  
  // change the npx of the second node
  if (*head) {
    Node *next = getNextAddress(originalHead, *head);
    (*head) -> xor_value = next;
  }
  
  // free the memory of the original head
  free(originalHead);
  
  // return successful
  return 1;
}

// If possible, removes the string after the string "after" and fills in the result buffer with its value. If successful return true, otherwise returns false
int remove_after(Node** head, const char *after, char *result) {
  // get the first two nodes
  Node *current = *head;
  Node *before = NULL;

  // traverse the list until we find the target string
  while (current != NULL) {    
    // check if the current node is "after"
    if (strcmp(current -> name, after) == 0) {
      
      Node *afterNode = getNextAddress(before, current);
      Node *next = NULL;
      Node *last = NULL;

      // if there is no node to remove, return unsuccessful
      if (afterNode) next = getNextAddress(current, afterNode);
      else return 0;

      // if there is a next node, get a pointer to the one after that
      if (next) last = getNextAddress(afterNode, next);

      // set result
      strcpy(result, afterNode -> name);

      // update the npx of the current node
      current -> xor_value = calculate_xor_value(before, next);
      
      // change the npx of the node after "after"
      if (next) next -> xor_value = calculate_xor_value(current, last);

      // free the memory of afterNode
      free(afterNode);

      // return success
      return 1;
    }

    // move on to the next node
    Node *next = getNextAddress(before, current);
    before = current;
    current = next;
  }
  return 0;
}

// If possible, removes the string before the string "before" and fills in the result buffer with its value. If successful return true, otherwise returns false.
int remove_before(Node** head, const char *before, char *result) {
  // get the first two nodes
  Node *current = *head;
  Node *beforeNode = NULL;

  // traverse the list until we find the target string
  while (current != NULL) {    
    // check if the current node is "before"
    if (strcmp(current -> name, before) == 0) {
      Node *next = NULL;
      Node *last = NULL;

      // if there is no node to remove, return unsuccessful
      if (beforeNode) next = getNextAddress(current, beforeNode);
      else return 0;
      
      // if there is a next node, get the address of the one after that
      if (next) last = getNextAddress(beforeNode, next);

      // set result
      strcpy(result, beforeNode -> name);

      // update the npx of the current node
      Node *tempNode = getNextAddress(beforeNode, current);
      current -> xor_value = calculate_xor_value(next, tempNode);

      // change the npx of the node before "before"
      if (next) next -> xor_value = calculate_xor_value(current, last);

      // check if the head was removed
      if (*head == beforeNode) *head = current;

      // free the memory of beforeNode
      free(beforeNode);

      // return success
      return 1;
    }

    // move on to the next node
    Node *next = getNextAddress(beforeNode, current);
    beforeNode = current;
    current = next;
  }

  // if element not found, return unsuccessful
  return 0;
}

// Removes all nodes and releases any memory allocated to the linked list
void free_all(Node** head) {
  // get the first two nodes
  Node *current = *head;
  Node *before = NULL;

  printf("Freeing all nodes \n");
  // free all the memory allocated to the list
  while (current != NULL) {
    Node *next = getNextAddress(before, current);

    if (before) printf("Freed %s\n", before -> name);
    free(before);
    before = NULL;

    before = current;
    current = next;
  }
  if (before) printf("Freed %s\n", before -> name);
  free(before);
  before = NULL;
  
  
  printf("-------------------------\n");
}

int main(void) {
  Node* head = NULL;
  char result[64];
  int success;

  insert_string(&head, "Alpha");
  insert_string(&head, "Bravo");
  insert_string(&head, "Charlie");
  printList(head);  

  success = insert_after(&head, "Alpha", "Delta");
  if (success) printf("Insertion successful\n");
  else printf("Insertion failed\n");

  success = insert_before(&head, "Charlie", "Echo");
  if (success) printf("Insertion successful\n");
  else printf("Insertion failed\n");
  
  success = remove_string(&head, result);
  if (success) printf("Removed \"%s\"\n", result);
  else printf("You can't do that\n");
  
  success = remove_after(&head, "Bravo", result);
  if (success) printf("Removed \"%s\"\n", result);
  else printf("You can't do that\n");

  success = remove_before(&head, "Bravo", result);
  if (success) printf("Removed \"%s\"\n", result);
  else printf("You can't do that\n");
  
  printList(head);
  free_all(&head);
  
  return 0;
}
