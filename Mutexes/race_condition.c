#include <pthread.h>
#include <stdio.h>

#define N 5

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void *swapper_fn_callback(void *args) {
  int *arr = (int *)args;
  while (1) {
    swap(&arr[0], &arr[N - 1]);
  }
  return NULL;
}

void *adder_fn_callback(void *args) {
  int *arr = (int *)args;
  while (1) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
      sum += arr[i];
    }
    printf("Sum: %d\n", sum);
  }
  return NULL;
}

int main() {
  static int data[N] = {1, 2, 4, 5, 7};

  pthread_t swapper, adder;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, 0);

  pthread_create(&swapper, &attr, swapper_fn_callback, (void *)data);
  pthread_create(&adder, &attr, adder_fn_callback, (void *)data);

  pthread_exit(0);
}
