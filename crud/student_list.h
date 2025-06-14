#include "../GLThreads/glthreads.h"
#include "refcount.h"
#include <pthread.h>
#include <stdint.h>

typedef struct student_ {

  uint32_t roll_no;
  uint32_t total_marks;
  ref_count_t ref_count;
  pthread_rwlock_t rw_lock;
  glthread_node_t node;
} student_t;

student_t *student_malloc(uint32_t roll_no);

typedef struct stud_lst_ {

  glthread_t lst;
  pthread_rwlock_t rw_lock;
} stud_lst_t;

void student_destroy(student_t *stud);

student_t *student_lst_lookup(stud_lst_t *stud_lst, uint32_t roll_no);

void student_lst_insert(stud_lst_t *stud_lst, student_t *stud);

student_t *student_lst_remove(stud_lst_t *stud_lst, uint32_t roll_no);
