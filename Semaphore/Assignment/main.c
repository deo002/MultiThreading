#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

pthread_t thread0, thread1;

sem_t *sem0;

void *one_thread_fn_callback(void *args) {

  printf("1\n");
  sem_post(sem0);

  return NULL;
}

void *two_thread_fn_callback(void *args) {

  sem_wait(sem0);
  printf("2\n");

  return NULL;
}

int main() {

  sem_unlink("/semaphore0");

  sem0 = sem_open("/semaphore0", O_CREAT, 0644, 0);
  if (sem0 == SEM_FAILED) {
    perror("sem init error");
  }

  pthread_create(&thread0, NULL, one_thread_fn_callback, NULL);
  pthread_create(&thread1, NULL, two_thread_fn_callback, NULL);

  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);

  if (sem_close(sem0) == -1) {
    perror("sem destroy error");
  }

  return 0;
}
