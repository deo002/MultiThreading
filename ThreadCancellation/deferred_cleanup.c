#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

pthread_t handles[N];

void function_args_cleanup(void *args) { free(args); }

void file_ptr_cleanup(void *ptr) { fclose(ptr); }

void *thread_fn_callback(void *args) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

  pthread_cleanup_push(function_args_cleanup, args);

  int *tid = (int *)args;
  char filename[100];
  sprintf(filename, "thread_%d.txt", *tid);

  FILE *ptr = fopen(filename, "a");

  if (ptr == NULL) {
    printf("Couldn't open file %s, errno = %d\n", filename, errno);
    pthread_exit(0);
  }

  pthread_cleanup_push(file_ptr_cleanup, ptr);

  while (1) {
    fprintf(ptr, "Thread with id %d running\n", *tid);
    fflush(ptr);
    sleep(1);
    pthread_testcancel();
  }

  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);
  return 0;
}

int main() {

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  for (int i = 0; i < N; i++) {
    int *tid = (int *)calloc(1, sizeof(int));
    *tid = i;
    pthread_create(&handles[i], &attr, thread_fn_callback, tid);
  }

  while (1) {
    int tid;
    printf("Enter thread id to be deleted:\n");
    scanf("%d", &tid);
    pthread_cancel(handles[tid]);
  }

  pthread_exit(0);
}
