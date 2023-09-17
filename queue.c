#include <stdlib.h>

// Circular queue structure
struct queue_s {
  void **buffer; // Array to store queue elements
  int front;     // Front index
  int rear;      // Rear index
  int size;      // Number of elements in the queue
  int capacity;  // Queue capacity
};

typedef struct queue_s *queue_t;

// Create a circular queue
queue_t createQueue(int capacity) {
  queue_t queue = malloc(sizeof(struct queue_s));
  if (queue == NULL) {
#ifdef errno
    errno = ENOMEM;
#endif
    return NULL;
  }

  queue->buffer = (void **)malloc(sizeof(void *) * capacity);
  if (queue->buffer == NULL) {
    free(queue);
#ifdef errno
    errno = ENOMEM;
#endif
    return NULL;
  }

  queue->front = 0;
  queue->rear = -1;
  queue->size = 0;
  queue->capacity = capacity;
  return queue;
}

// Destroy the circular queue
void destroyQueue(queue_t queue) {
  if (queue) {
    free(queue->buffer);
    free(queue);
  }
}

// Check if the queue is empty
int isEmpty(queue_t queue) { return queue->size == 0; }

// Check if the queue is full
int isFull(queue_t queue) { return queue->size == queue->capacity; }

// Enqueue operation
int enqueue(queue_t queue, void *data) {
  if (isFull(queue)) {
#ifdef errno
    errno = EOVERFLOW;
#endif
    return 1;
  }

  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->buffer[queue->rear] = data;
  queue->size++;
  return 0;
}

// Dequeue operation
int dequeue(queue_t queue) {
  if (isEmpty(queue)) {
#ifdef errno
    errno = EAGAIN;
#endif
    return 1;
  }

  queue->front = (queue->front + 1) % queue->capacity;
  queue->size--;
  return 0;
}

// Get the front element of the queue
void *front(queue_t queue) {
  if (isEmpty(queue)) {
#ifdef errno
    errno = EAGAIN;
#endif
    return NULL;
  }

  return queue->buffer[queue->front];
}
