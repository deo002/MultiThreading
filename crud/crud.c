#include "refcount.h"
#include "student_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROLL_NO 10
#define INCR 5

enum { READER_TH, UPDATE_TH, CREATE_TH, DELETE_TH, THREAD_TYPE_MAX } th_type_t;

static pthread_t threads[THREAD_TYPE_MAX];
static uint32_t loop_count[THREAD_TYPE_MAX];

static stud_lst_t stud_lst; /* Container Object */

static void *reader_fn(void *arg) {
  student_t *stud;
  uint32_t roll_no;

  while (1) {

    roll_no = rand() % MAX_ROLL_NO;

    pthread_rwlock_rdlock(&stud_lst.rw_lock);

    stud = student_lst_lookup(&stud_lst, roll_no);

    if (stud == NULL) {
      printf("READER_TH: Student with roll_no %u doesnot exist\n", roll_no);
      pthread_rwlock_unlock(&stud_lst.rw_lock);
      continue;
    }

    thread_using_object(&stud->ref_count);

    pthread_rwlock_rdlock(&stud->rw_lock);
    pthread_rwlock_unlock(&stud_lst.rw_lock);

    printf("READER_TH: Read done for student with roll no %u\n", roll_no);

    pthread_rwlock_unlock(&stud->rw_lock);

    if (thread_done_using_object(&stud->ref_count)) {
      student_destroy(stud);

      printf("READER_TH: Student with roll no %u removed from list\n", roll_no);
    }
  }

  return NULL;
}

static void *update_fn(void *arg) {
  student_t *stud;
  uint32_t roll_no;

  while (1) {

    roll_no = rand() % MAX_ROLL_NO;

    pthread_rwlock_rdlock(&stud_lst.rw_lock);

    stud = student_lst_lookup(&stud_lst, roll_no);

    if (stud == NULL) {
      printf("UPDATE_TH: Student with roll_no %u doesnot exist\n", roll_no);
      pthread_rwlock_unlock(&stud_lst.rw_lock);
      continue;
    }

    thread_using_object(&stud->ref_count);

    pthread_rwlock_wrlock(&stud->rw_lock);
    pthread_rwlock_unlock(&stud_lst.rw_lock);

    stud->total_marks += INCR;
    stud->total_marks = stud->total_marks % 100;
    printf("UPDATE_TH: Update done for student with roll no %u\n", roll_no);

    pthread_rwlock_unlock(&stud->rw_lock);

    if (thread_done_using_object(&stud->ref_count)) {
      student_destroy(stud);

      printf("READER_TH: Student with roll no %u removed from list\n", roll_no);
    }
  }

  return NULL;
}

static void *create_fn(void *arg) {

  uint32_t roll_no;
  student_t *stud;

  while (1) {

    roll_no = rand() % MAX_ROLL_NO;

    pthread_rwlock_wrlock(&stud_lst.rw_lock);

    student_t *student = student_lst_lookup(&stud_lst, roll_no);
    if (student != NULL) {
      printf("CREATE_TH: Student with roll no %u already exists\n", roll_no);
      pthread_rwlock_unlock(&stud_lst.rw_lock);
      continue;
    }

    stud = student_malloc(roll_no);

    student_lst_insert(&stud_lst, stud);
    ref_count_inc(&stud->ref_count);
    printf("CREATE_TH: Student with roll no %u created successfully\n",
           stud->roll_no);

    pthread_rwlock_unlock(&stud_lst.rw_lock);
  }
  return NULL;
}

static void *delete_fn(void *arg) {

  uint32_t roll_no;

  while (1) {

    roll_no = rand() % MAX_ROLL_NO;

    pthread_rwlock_wrlock(&stud_lst.rw_lock);

    student_t *stud = student_lst_remove(&stud_lst, roll_no);
    if (stud == NULL) {
      printf("DELETE_TH: Student with roll no %u doesnt exist\n", roll_no);
      pthread_rwlock_unlock(&stud_lst.rw_lock);
      continue;
    }

    thread_using_object(&stud->ref_count);
    assert(!ref_count_des(&stud->ref_count));
    if (thread_done_using_object(&stud->ref_count)) {
      student_destroy(stud);
      printf("DELETE_TH: Student with roll no %u deleted successfully\n",
             stud->roll_no);
    } else {
      printf("DELETE_TH: Student with roll no %u deletion defered\n",
             stud->roll_no);
    }

    pthread_rwlock_unlock(&stud_lst.rw_lock);
  }

  return NULL;
}

int main(int argc, char **argv) {

  init_glthread(&stud_lst.lst, OFFSETOF(student_t, node));
  pthread_rwlock_init(&stud_lst.rw_lock, NULL);

  loop_count[READER_TH] = 0;
  loop_count[UPDATE_TH] = 0;
  loop_count[CREATE_TH] = 0;
  loop_count[DELETE_TH] = 0;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_create(&threads[READER_TH], &attr, reader_fn, NULL);
  pthread_create(&threads[UPDATE_TH], &attr, update_fn, NULL);
  pthread_create(&threads[CREATE_TH], &attr, create_fn, NULL);
  pthread_create(&threads[DELETE_TH], &attr, delete_fn, NULL);

  pthread_exit(0);
  return 0;
}
