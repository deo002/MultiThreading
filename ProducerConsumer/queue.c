#include "queue.h"

queue_t *init_queue() {
  queue_t *q = calloc(1, sizeof(queue_t));

  q->head = NULL;
  q->tail = NULL;
  q->size = 0;

  pthread_mutex_init(&q->mutex, NULL);
  pthread_cond_init(&q->cv, NULL);

  return q;
}

void cleanup(queue_t *q) {
  pthread_mutex_destroy(&q->mutex);
  pthread_cond_destroy(&q->cv);
  free(q);
}

bool is_empty(queue_t *q) {
  if (q->size == 0) {
    return true;
  }
  return false;
}

bool is_full(queue_t *q) {
  if (q->size == MAX_SIZE) {
    return true;
  }
  return false;
}

void enqueue(queue_t *q, queue_node_t *node) {
  if (q->head == NULL) {
    q->head = q->tail = node;
  } else {
    q->tail->next = node;
    q->tail = node;
  }
  q->size++;
}

queue_node_t *dequeue(queue_t *q) {
  queue_node_t *node = q->head;
  if (q->head == q->tail) {
    q->head = q->tail = NULL;
  } else {
    q->head = q->head->next;
  }
  node->next = NULL;
  q->size--;
  return node;
}
