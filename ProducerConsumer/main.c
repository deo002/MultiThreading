#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef struct thread_args {
  char name[64];
  queue_t *q;
} args_t;

void *producer_callback_fn(void *args) {
  args_t *_args = (args_t *)args;

  queue_node_t *nodes[MAX_SIZE];
  for (int i = 0; i < MAX_SIZE; i++) {
    nodes[i] = calloc(1, sizeof(queue_node_t));
    if (nodes[i] == NULL) {
      perror("Unable to allocate memory: ");
    }
  }

  printf("Thread %s waiting to lock the Queue\n", _args->name);
  pthread_mutex_lock(&_args->q->mutex);
  printf("Thread %s locks the Queue\n", _args->name);

  for (int i = 0; i < MAX_SIZE; i++) {
    nodes[i]->data = i;
    nodes[i]->next = NULL;

    while (is_full(_args->q)) {
      printf("Thread %s blocks itself, Q is already Full\n", _args->name);
      pthread_cond_wait(&_args->q->cv, &_args->q->mutex);
      printf("Thread %s wakes up, checking the Queue status again\n",
             _args->name);
    }

    printf("Thread %s produces new integer %d\n", _args->name, i);
    enqueue(_args->q, nodes[i]);
    printf("Thread %s pushed integer %d in Queue, Queue size = %d\n",
           _args->name, i, _args->q->size);
  }
  printf("Thread %s Filled up the Queue, signalling and releasing lock\n",
         _args->name);
  pthread_cond_signal(&_args->q->cv);
  pthread_mutex_unlock(&_args->q->mutex);
  printf("Thread %s finished, and exit\n", _args->name);
  return NULL;
}

void *consumer_callback_fn(void *args) {
  args_t *_args = (args_t *)args;
  printf("Thread %s waiting to lock the Queue\n", _args->name);
  pthread_mutex_lock(&_args->q->mutex);
  printf("Thread %s locks the Queue\n", _args->name);

  for (int i = 0; i < MAX_SIZE; i++) {

    while (is_empty(_args->q)) {
      printf("Thread %s blocks itself, Q is already empty\n", _args->name);
      pthread_cond_wait(&_args->q->cv, &_args->q->mutex);
      printf("Thread %s wakes up, checking the Queue status again\n",
             _args->name);
    }

    queue_node_t *node = dequeue(_args->q);
    free(node);
    printf("Thread %s consumes an integer %d, Queue size = %d\n", _args->name,
           i, _args->q->size);
  }
  pthread_cond_signal(&_args->q->cv);
  printf(
      "Thread %s Drains the entire Queue, sending signal to Blocking Threads\n",
      _args->name);
  printf("Thread %s releasing lock\n", _args->name);
  pthread_mutex_unlock(&_args->q->mutex);
  printf("Thread %s finished, and exit\n", _args->name);
  return NULL;
}

int main() {

  pthread_t producer1, producer2, producer3, consumer1, consumer2, consumer3;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  queue_t *q = init_queue();
  args_t *args = calloc(6, sizeof(args_t));

  strcpy(args[0].name, "Producer 1");
  args[0].q = q;

  strcpy(args[1].name, "Producer 2");
  args[1].q = q;

  strcpy(args[2].name, "Consumer 1");
  args[2].q = q;

  strcpy(args[3].name, "Consumer 2");
  args[3].q = q;

  strcpy(args[4].name, "Producer 3");
  args[4].q = q;

  strcpy(args[5].name, "Consumer 3");
  args[5].q = q;

  pthread_create(&producer1, &attr, producer_callback_fn, (void *)&args[0]);
  pthread_create(&producer2, &attr, producer_callback_fn, (void *)&args[1]);

  pthread_create(&consumer1, &attr, consumer_callback_fn, (void *)&args[2]);
  pthread_create(&consumer2, &attr, consumer_callback_fn, (void *)&args[3]);

  pthread_create(&producer3, &attr, producer_callback_fn, (void *)&args[4]);
  pthread_create(&consumer3, &attr, consumer_callback_fn, (void *)&args[5]);

  pthread_join(producer1, NULL);
  pthread_join(producer2, NULL);
  pthread_join(consumer1, NULL);
  pthread_join(consumer2, NULL);
  pthread_join(producer3, NULL);
  pthread_join(consumer3, NULL);

  cleanup(q);

  pthread_exit(0);
}
