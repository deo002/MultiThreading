#ifndef __SEMA__
#define __SEMA__

#include <pthread.h>
#include <stdlib.h>

typedef struct sema {
  int permit_counter;
  pthread_mutex_t mutex;
  pthread_cond_t cv;
} sema_t;

sema_t *sema_get_new_semaphore();

void sema_init(sema_t *sema, int count);

void sema_wait(sema_t *sema);

void sema_post(sema_t *sema);

void sema_destroy(sema_t *sema);

int sema_getvalue(sema_t *sema);

#endif
