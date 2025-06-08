#include "refcount.h"

void ref_count_init(ref_count_t *ref_count) {
  pthread_mutex_init(&ref_count->mutex, NULL);
}

void ref_count_inc(ref_count_t *ref_count) {
  pthread_mutex_lock(&ref_count->mutex);
  ref_count->ref_count++;
  pthread_mutex_unlock(&ref_count->mutex);
}

bool ref_count_des(ref_count_t *ref_count) {
  pthread_mutex_lock(&ref_count->mutex);
  ref_count->ref_count--;
  int rc = (ref_count->ref_count == 0) ? true : false;
  pthread_mutex_unlock(&ref_count->mutex);

  return rc;
}

void ref_count_destroy(ref_count_t *ref_count) {
  assert(ref_count->ref_count == 0);
  pthread_mutex_destroy(&ref_count->mutex);
}

void thread_using_object(ref_count_t *ref_count) { ref_count_inc(ref_count); }

bool thread_done_using_object(ref_count_t *ref_count) {
  return ref_count_des(ref_count);
}
