#ifndef __GLTHREADS__
#define __GLTHREADS__

#include<stdlib.h>

typedef struct glthread_node_ {
	struct glthread_node_ *left;
	struct glthread_node_ *right;
}glthread_node_t;

typedef struct glthread_ {
	glthread_node_t *head;
	size_t  offset;

}glthread_t;

void glthread_add(glthread_t *list, glthread_node_t *glnode);

void glthread_remove(glthread_t *head, glthread_node_t *glnode);

void init_glthread(glthread_t *glthread, unsigned int offset);

#define OFFSETOF(struct_name, field_name) ((size_t)&(((struct_name *)0)->field_name))

#endif
