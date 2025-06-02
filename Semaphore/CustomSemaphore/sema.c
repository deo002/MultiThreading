#include "sema.h"

sema_t *sema_get_new_semaphore() {
  sema_t *sem = calloc(1, sizeof(sema_t));
  return sem;
}

void sema_init(sema_t *sema, int count) {
  sema->permit_counter = count;
  pthread_cond_init(&sema->cv, NULL);
  pthread_mutex_init(&sema->mutex, NULL);
}

void sema_wait(sema_t *sema) {
  pthread_mutex_lock(&sema->mutex);

  sema->permit_counter--;

  while (sema->permit_counter < 0) {
    pthread_cond_wait(&sema->cv, &sema->mutex);
  }

  pthread_mutex_unlock(&sema->mutex);
}

void sema_post(sema_t *sema) {
  pthread_mutex_lock(&sema->mutex);

  sema->permit_counter++;
  pthread_cond_signal(&sema->cv);

  pthread_mutex_unlock(&sema->mutex);
}

void sema_destroy(sema_t *sema) {
  pthread_cond_destroy(&sema->cv);
  pthread_mutex_destroy(&sema->mutex);
  free(sema);
}

int sema_getvalue(sema_t *sema) { return sema->permit_counter; }
