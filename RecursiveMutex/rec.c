#include "rec.h"
#include <assert.h>

void rec_mutex_init(rec_mutex_t *rec_mutex) {
  rec_mutex->n = 0;
  rec_mutex->n_waited = 0;
  rec_mutex->locking_thread = NULL;
  pthread_mutex_init(&rec_mutex->state_mutex, NULL);
  pthread_cond_init(&rec_mutex->cv, NULL);
}

void rec_mutex_lock(rec_mutex_t *rec_mutex) {
  pthread_mutex_lock(&rec_mutex->state_mutex);

  if (rec_mutex->locking_thread == NULL) {
    rec_mutex->locking_thread = pthread_self();
    rec_mutex->n++;
  } else if (rec_mutex->locking_thread == pthread_self()) {
    rec_mutex->n++;
  } else {
    rec_mutex->n_waited++;
    pthread_cond_wait(&rec_mutex->cv, &rec_mutex->state_mutex);
    rec_mutex->n_waited--;
    rec_mutex->locking_thread = pthread_self();
    rec_mutex->n++;
  }

  pthread_mutex_unlock(&rec_mutex->state_mutex);
}

void rec_mutex_unlock(rec_mutex_t *rec_mutex) {
  pthread_mutex_lock(&rec_mutex->state_mutex);

  assert(rec_mutex->locking_thread == NULL);
  assert(rec_mutex->locking_thread != pthread_self());

  rec_mutex->n--;

  if (rec_mutex->n == 0 && rec_mutex->n_waited > 0) {
    pthread_cond_signal(&rec_mutex->cv);
  }

  pthread_mutex_unlock(&rec_mutex->state_mutex);
}

void rec_mutex_destroy(rec_mutex_t *rec_mutex) {
  pthread_mutex_destroy(&rec_mutex->state_mutex);
  pthread_cond_destroy(&rec_mutex->cv);
  free(rec_mutex);
}
