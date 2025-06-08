#include "student_list.h"
#include "refcount.h"

student_t *student_malloc(uint32_t roll_no) {
  student_t *new_student = calloc(1, sizeof(student_t));

  new_student->roll_no = roll_no;
  new_student->total_marks = 0;
  pthread_rwlock_init(&new_student->rw_lock, NULL);
  ref_count_init(&new_student->ref_count);

  new_student->node.left = new_student->node.right = NULL;

  return new_student;
}

student_t *student_lst_lookup(stud_lst_t *stud_lst, uint32_t roll_no) {
  glthread_node_t *ptr = stud_lst->lst.head;
  do {
    if (ptr == NULL) {
      return NULL;
    }

    student_t *stud = (student_t *)((char *)ptr - stud_lst->lst.offset);
    if (stud->roll_no == roll_no) {
      return stud;
    }
    ptr = ptr->right;
  } while (ptr != stud_lst->lst.head);

  return NULL;
}

void student_lst_insert(stud_lst_t *stud_lst, student_t *stud) {
  glthread_add(&stud_lst->lst, &stud->node);
}

student_t *student_lst_remove(stud_lst_t *stud_lst, uint32_t roll_no) {
  glthread_node_t *ptr = stud_lst->lst.head;
  student_t *student = NULL;
  do {
    if (ptr == NULL)
      return NULL;
    student_t *s = (student_t *)((char *)ptr - stud_lst->lst.offset);
    if (s->roll_no == roll_no) {
      student = s;
      break;
    }
    ptr = ptr->right;
  } while (ptr != stud_lst->lst.head);

  glthread_remove(&stud_lst->lst, ptr);

  return student;
}

void student_destroy(student_t *stud) {
  pthread_rwlock_destroy(&stud->rw_lock);
  ref_count_destroy(&stud->ref_count);
  free(stud);
}
