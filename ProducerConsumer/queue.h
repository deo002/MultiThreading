#ifndef __QUEUE__
#define __QUEUE__

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 5

typedef struct queue_node {
  int data;
  struct queue_node *next;
} queue_node_t;

typedef struct queue {
  queue_node_t *head;
  queue_node_t *tail;
  int size;
  pthread_mutex_t mutex;
  pthread_cond_t cv;
} queue_t;

queue_t *init_queue();

bool is_empty(queue_t *);

bool is_full(queue_t *);

// we cannot do malloc or any other call which brings the thread to a halt after
// mutex is initialized because mutext_unlock will be never called then
void enqueue(queue_t *, queue_node_t *);

queue_node_t *dequeue(queue_t *);

void cleanup(queue_t *);
#endif
