#ifndef __REF_COUNT__
#define __REF_COUNT__

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * The purpose of reference count object is to keep track of the
 * number of entities(data structure and threads) referencing the object
 *
 * Reference count objects help in premature deletion of objects in
 * multithreaded environment
 *
 * Initialisation:
 * When reference counted object is malloc'd, it's count is set to zero
 *
 * Increment Rules:
 * Reference counted object is inserted into container(tree/map/linked lisy)
 * When thread get an access to the object
 * Any other data structure in program points to the referenced count object
 *
 * Decrement Rules:
 * Reference counted object is removed from container
 * When thread decides not to use an object anymore
 * Any other data structure in the system stops pointing to reference counted
 * object
 *
 * */
typedef struct ref_count {

  uint32_t ref_count;

  /* OS X does not support spinlocks, so doing it with mutex instead */
  pthread_mutex_t mutex;
} ref_count_t;

void ref_count_init(ref_count_t *ref_count);

void ref_count_inc(ref_count_t *ref_count);

bool ref_count_des(ref_count_t *ref_count);

void ref_count_destroy(ref_count_t *ref_count);

void thread_using_object(ref_count_t *ref_count);

bool thread_done_using_object(ref_count_t *ref_count);

#endif
