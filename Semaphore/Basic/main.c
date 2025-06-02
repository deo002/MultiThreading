#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * A semaphore with a permit number as 1 is same as a mutex.
 *
 * The only difference is that sem_post from another thread can unlock the
 * thread
 *
 * If the value of permit count after decrement is less than 0, then the
 * calling thread is blocked
 *
 * */

#define NUM_THREADS 5

#define PERMIT_NUMBER 2

pthread_t handles[NUM_THREADS];

sem_t *sem;

void *thread_fn_callback(void *args) {
  char *name = (char *)args;

  sem_wait(sem);

  for (int i = 0; i < 5; i++) {
    printf("%s executing critical section\n", name);
    sleep(1);
  }

  sem_post(sem);

  return NULL;
}

void thread_create(pthread_t *handle, char *thread_name) {
  int rc = pthread_create(handle, NULL, thread_fn_callback, thread_name);
  if (rc != 0) {
    perror("thread creation");
    exit(EXIT_FAILURE);
  }
}

int main() {

  if (sem_unlink("/semaphore") == -1) {
    perror("sem_unlink");
    exit(EXIT_FAILURE);
  }

  if ((sem = sem_open("/semaphore", O_CREAT, 0644, PERMIT_NUMBER)) ==
      SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  thread_create(&handles[0], "thread0");
  thread_create(&handles[1], "thread1");
  thread_create(&handles[2], "thread2");
  thread_create(&handles[3], "thread3");
  thread_create(&handles[4], "thread4");

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(handles[i], NULL);
  }

  if (sem_close(sem) == -1) {
    perror("sem_close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
