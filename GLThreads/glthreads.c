#include "glthreads.h"
#include<stdio.h>

/* implementation of circular doubly linked list */

/* the new node is added at the head of the list */
void glthread_add(glthread_t *lst, glthread_node_t *node) {
	node->right = lst->head;
	if(lst->head == NULL) {
		lst->head = node;
	} else {
		glthread_node_t *temp = lst->head->left;
		node->right = lst->head;
		lst->head->left = node;
		if(temp == NULL) {
			node->left = lst->head;
			lst->head->right = node;	
		}
		else {
			node->left = temp;
			temp->right = node;
		}	
		lst->head = node;
	}
}

void glthread_remove(glthread_t *lst, glthread_node_t *node) {
	if(node == NULL) {
		return;
	}	
	glthread_node_t *next = node->right;
	glthread_node_t *prev = node->left;

	if(lst->head == node) {
		lst->head = next;
	}
	
	node->left = NULL;
	node->right = NULL;

	if(next != NULL && prev != NULL) {
		if(next != prev) {
			next->left = prev;	
			prev->right = next;
		} else {
			next->left = next->right = NULL;
		}
	}
}

void init_glthread(glthread_t *glthread, unsigned int offset) {
	glthread->head = NULL;
	glthread->offset = offset;
}





