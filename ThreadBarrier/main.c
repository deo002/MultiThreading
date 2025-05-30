#include "threadbarrier.h"
#include <pthread.h>
#include <stdio.h>

#define THREAD_BARRIER_THRESHOLD 3

static pthread_t handles[THREAD_BARRIER_THRESHOLD];

static th_barrier_t barrier;

void *thread_fn_callback(void *arg) {

  thread_barrier_wait(&barrier, (char *)arg);

  printf("Ist barricade cleared by thread %s\n", (char *)arg);

  thread_barrier_wait(&barrier, (char *)arg);

  printf("2nd barricade cleared by thread %s\n", (char *)arg);

  thread_barrier_wait(&barrier, (char *)arg);

  printf("3rd barricade cleared by thread %s\n", (char *)arg);

  pthread_exit(0);
  return NULL;
}

int main() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  thread_barrier_init(&barrier, THREAD_BARRIER_THRESHOLD);

  static const char *th1 = "th1";
  pthread_create(&handles[0], &attr, thread_fn_callback, (void *)th1);

  static const char *th2 = "th2";
  pthread_create(&handles[1], &attr, thread_fn_callback, (void *)th2);

  static const char *th3 = "th3";
  pthread_create(&handles[2], &attr, thread_fn_callback, (void *)th3);

  pthread_join(handles[0], NULL);
  pthread_join(handles[1], NULL);
  pthread_join(handles[2], NULL);

  thread_barrier_destroy(&barrier);

  pthread_exit(0);
}
