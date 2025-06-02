#include "sema.h"
#include <pthread.h>
#include <stdio.h>

pthread_t thread0, thread1;

sema_t *sem;

/*
 * Semaphores can be named or unnamed
 *
 * Named semaphores can be used to synchronize between related or unrelated
 * processes, threads etc or any entities infact
 *
 *
 * Unnamed semaphores are used to synchronize between related processes and
 * threads
 *
 * Whenever we can theoretically show that one of the threads waiting on a
 * semaphore may never get a chance to resume execution, then we say that the
 * solution is lacking the property of bounded wait and it's called a weak
 * semaphore
 *
 * If we can deploy a way such that blocked threads are unblocked in a FIFO way
 * per signal, then such a semaphore is called a strong semaphore *
 *
 * */

void *one_thread_fn_callback(void *args) {

  printf("1\n");
  sema_post(sem);

  return NULL;
}

void *two_thread_fn_callback(void *args) {

  sema_wait(sem);
  printf("2\n");

  return NULL;
}

int main() {

  sem = sema_get_new_semaphore();
  sema_init(sem, 0);

  pthread_create(&thread0, NULL, one_thread_fn_callback, NULL);
  pthread_create(&thread1, NULL, two_thread_fn_callback, NULL);

  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);

  sema_destroy(sem);

  return 0;
}
