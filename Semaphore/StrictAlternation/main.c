#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t odd, even;

sem_t *sem0, *sem1;

void *odd_thread_fn_callback(void *args) {
  for (int i = 1; i <= 15; i += 2) {
    sem_wait(sem0);
    printf("%d\n", i);
    sem_post(sem1);
  }
  return NULL;
}

void *even_thread_fn_callback(void *args) {
  for (int i = 0; i <= 15; i += 2) {
    printf("%d\n", i);
    sem_post(sem0);
    sem_wait(sem1);
  }
  return NULL;
}

int main() {

  sem_unlink("/semaphore0");

  sem_unlink("/semaphore1");

  sem0 = sem_open("/semaphore0", O_CREAT, 0644, 0);
  if (sem0 == SEM_FAILED) {
    perror("semaphore init failed");
    exit(EXIT_FAILURE);
  }

  sem1 = sem_open("/semaphore1", O_CREAT, 0644, 0);
  if (sem1 == SEM_FAILED) {
    perror("semaphore init failed");
    exit(EXIT_FAILURE);
  }

  pthread_create(&even, NULL, even_thread_fn_callback, NULL);
  pthread_create(&odd, NULL, odd_thread_fn_callback, NULL);

  pthread_join(odd, NULL);
  pthread_join(even, NULL);

  if (sem_close(sem0) == -1) {
    perror("sem_close");
    exit(EXIT_FAILURE);
  }

  if (sem_close(sem1) == -1) {
    perror("sem_close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
