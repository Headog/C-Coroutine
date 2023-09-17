#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <stdlib.h>

// Circular queue structure
typedef struct {
  void **buffer; // Array to store queue elements
  int front;     // Front index
  int rear;      // Rear index
  int size;      // Number of elements in the queue
  int capacity;  // Queue capacity
} queue_s;

typedef queue_s *queue_t;

// Function prototypes

// Create a circular queue
queue_t createQueue(int capacity);

// Destroy the circular queue
void destroyQueue(queue_t queue);

// Check if the queue is empty
bool isEmpty(queue_t queue);

// Check if the queue is full
bool isFull(queue_t queue);

// Enqueue operation
bool enqueue(queue_t queue, void *data);

// Dequeue operation
bool dequeue(queue_t queue);

// Get the front element of the queue
void *front(queue_t queue);

#endif // _QUEUE_H_
