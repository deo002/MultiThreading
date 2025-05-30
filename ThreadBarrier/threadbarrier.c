#include "threadbarrier.h"

void thread_barrier_init(th_barrier_t *barrier, uint32_t threshold_count) {
  barrier->threshold_count = threshold_count;
  barrier->curr_wait_count = 0;
  barrier->is_ready_again = true;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->cv, NULL);
  pthread_cond_init(&barrier->busy_cv, NULL);
}

void thread_barrier_destroy(th_barrier_t *barrier) {
  pthread_mutex_destroy(&barrier->mutex);
  pthread_cond_destroy(&barrier->cv);
}

void thread_barrier_wait(th_barrier_t *barrier, char *th_name) {
  pthread_mutex_lock(&barrier->mutex);
  printf("%s thread entered the barrier\n", th_name);
  while (!barrier->is_ready_again) {
    printf(
        "Thread %s has to wait because barrier is busy handling the previous "
        "batch\n",
        th_name);
    pthread_cond_wait(&barrier->busy_cv, &barrier->mutex);
  }

  barrier->curr_wait_count++;
  while (barrier->is_ready_again &&
         barrier->curr_wait_count < barrier->threshold_count) {
    printf("Thread %s waiting at the barrier till threshold numbers of threads "
           "arrive\n",
           th_name);
    pthread_cond_wait(&barrier->cv, &barrier->mutex);
  }

  printf("Thread %s has passed the barrier\n", th_name);
  barrier->is_ready_again = false;
  barrier->curr_wait_count--;
  pthread_cond_signal(&barrier->cv);
  printf("Thread %s signalled another thread to cross the barrier\n\n",
         th_name);
  if (barrier->curr_wait_count == 0) {
    barrier->is_ready_again = true;
    pthread_cond_broadcast(&barrier->busy_cv);
  }

  pthread_mutex_unlock(&barrier->mutex);
}
