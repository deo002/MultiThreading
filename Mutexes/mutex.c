#include <pthread.h>
#include <stdio.h>

/*
 * If t1 locks a mutex, then t1 should be the one unlocking it.
 *
 * A thread cannot unlock an already unlocked mutex - Undefined Behaviour
 *
 * If thread t tries to double lock a mutex, it will be self deadlocked
 *
 * Mutexed must be unlocked in LIFO order
 *
 * There are two code paradigms for using a mutex,
 * one is code blocking and another is data blocking
 *
 * Order of locking doesn't matter, but whatever order we choose,
 * it must be maintained throughout the application
 *
 * */

#define N 5

pthread_mutex_t mutex;

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void *swapper_fn_callback(void *args) {
  int *arr = (int *)args;
  while (1) {
    pthread_mutex_lock(&mutex);
    swap(&arr[0], &arr[N - 1]);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *adder_fn_callback(void *args) {
  int *arr = (int *)args;
  while (1) {
    pthread_mutex_lock(&mutex);
    int sum = 0;
    for (int i = 0; i < N; i++) {
      sum += arr[i];
    }
    printf("Sum: %d\n", sum);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main() {
  static int data[N] = {1, 2, 4, 5, 7};

  pthread_t swapper, adder;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, 0);

  pthread_mutex_init(&mutex, 0);

  pthread_create(&swapper, &attr, swapper_fn_callback, (void *)data);
  pthread_create(&adder, &attr, adder_fn_callback, (void *)data);

  pthread_exit(0);
}
