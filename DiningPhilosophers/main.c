#include "din_ph.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_PHILOSOPHER 10

static phil_t phil[N_PHILOSOPHER];
static spoon_t spoon[N_PHILOSOPHER];

static spoon_t *phil_get_right_spoon(phil_t *phil) {

  int phil_id = phil->phil_id;

  if (phil_id == 0) {
    return &spoon[N_PHILOSOPHER - 1];
  }
  return &spoon[phil_id - 1];
}

static spoon_t *phil_get_left_spoon(phil_t *phil) {

  return &spoon[phil->phil_id];
}

void phil_eat(phil_t *phil) {

  spoon_t *left_spoon = phil_get_left_spoon(phil);
  spoon_t *right_spoon = phil_get_right_spoon(phil);

  assert(left_spoon->phil == phil);
  assert(right_spoon->phil == phil);
  assert(left_spoon->is_used == true);
  assert(right_spoon->is_used == true);
  phil->eat_count++;
  printf("Phil %d eats with spoon [%d, %d] for %d times\n", phil->phil_id,
         left_spoon->spoon_id, right_spoon->spoon_id, phil->eat_count);
  sleep(1);
}

void philosopher_release_both_spoons(phil_t *phil) {

  spoon_t *left_spoon = phil_get_left_spoon(phil);
  spoon_t *right_spoon = phil_get_right_spoon(phil);

  pthread_mutex_lock(&left_spoon->mutex);
  pthread_mutex_lock(&right_spoon->mutex);
  printf("Philosopher %d releases both spoons %d & %d\n", phil->phil_id,
         left_spoon->spoon_id, right_spoon->spoon_id);
  left_spoon->is_used = false;
  left_spoon->phil = NULL;

  right_spoon->is_used = false;
  right_spoon->phil = NULL;

  pthread_cond_signal(&left_spoon->cv);
  pthread_cond_signal(&right_spoon->cv);

  pthread_mutex_unlock(&right_spoon->mutex);
  pthread_mutex_unlock(&left_spoon->mutex);
}

bool philosopher_get_access_both_spoons(phil_t *phil) {

  spoon_t *left_spoon = phil_get_left_spoon(phil);
  spoon_t *right_spoon = phil_get_right_spoon(phil);

  bool gets_access = true;

  pthread_mutex_lock(&left_spoon->mutex);

  printf("Philosopher %d tries to access left spoon %d\n", phil->phil_id,
         left_spoon->spoon_id);
  while (left_spoon->is_used) {
    printf("Philosopher %d waiting for left spoon %d to be free\n",
           phil->phil_id, left_spoon->spoon_id);
    pthread_cond_wait(&left_spoon->cv, &left_spoon->mutex);
  }

  left_spoon->is_used = true;
  left_spoon->phil = phil;
  printf("Philosopher %d acquired left spoon %d\n", phil->phil_id,
         left_spoon->spoon_id);
  pthread_mutex_unlock(&left_spoon->mutex);

  pthread_mutex_lock(&right_spoon->mutex);

  if (right_spoon->is_used) {
    printf("Philosopher %d failed to acquire right spoon %d, leaving left "
           "spoon %d as well\n",
           phil->phil_id, right_spoon->spoon_id, left_spoon->spoon_id);
    pthread_mutex_lock(&left_spoon->mutex);
    left_spoon->is_used = false;
    left_spoon->phil = NULL;
    pthread_mutex_unlock(&left_spoon->mutex);
    gets_access = false;
  } else {
    printf("Philosopher %d successfully acquired both spoons %d & %d\n",
           phil->phil_id, left_spoon->spoon_id, right_spoon->spoon_id);
    right_spoon->is_used = true;
    right_spoon->phil = phil;
  }

  pthread_mutex_unlock(&right_spoon->mutex);

  return gets_access;
}

void *philosopher_fn(void *arg) {
  phil_t *phil = (phil_t *)arg;

  while (1) {
    if (!philosopher_get_access_both_spoons(phil)) {
      sleep(1);
    } else {
      phil_eat(phil);
      philosopher_release_both_spoons(phil);
    }
  }
}

int main(int argc, char **argv) {

  int i = 0;
  pthread_attr_t attr;

  for (i = 0; i < N_PHILOSOPHER; i++) {

    spoon[i].spoon_id = i;
    spoon[i].is_used = false;
    spoon[i].phil = NULL;
    pthread_mutex_init(&spoon[i].mutex, NULL);
    pthread_cond_init(&spoon[i].cv, NULL);
  }

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  for (i = 0; i < N_PHILOSOPHER; i++) {
    phil[i].phil_id = i;
    phil[i].eat_count = 0;
    pthread_create(&phil[i].thread_handle, &attr, philosopher_fn,
                   (void *)&phil[i]);
  }

  pthread_exit(0);
  return 0;
}
