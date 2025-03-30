#include"glthreads.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct emp_ {
	char name[30];
	unsigned int salary;
	char designation[30];
	unsigned int emp_id;
	glthread_node_t glnode;
} emp_t; 

void
print_emp_details(emp_t *emp){
	printf("Employee name = %s\n", emp->name);
	printf("salary = %u\n", emp->salary);
	printf("designation = %s\n", emp->designation);
	printf("emp_id = %u\n", emp->emp_id);
}

int main(int argc, char **argv) {

	emp_t *emp1  = calloc(1, sizeof(emp_t));
	strncpy(emp1->name, "Neha", strlen("Neha"));
	emp1->salary = 50000;
	strncpy(emp1->designation, "HR", strlen("HR"));
	emp1->emp_id = 21;
	emp1->glnode.left = emp1->glnode.right = NULL;

	emp_t *emp2  = calloc(1, sizeof(emp_t));
	strncpy(emp2->name, "Abhishek", strlen("Abhishek"));
	emp1->salary = 150000;
	strncpy(emp2->designation, "SE3", strlen("SE3"));
	emp2->emp_id = 32;
	emp2->glnode.left = emp2->glnode.right = NULL;

	emp_t *emp3  = calloc(1, sizeof(emp_t));
	strncpy(emp3->name, "Arun", strlen("Arun"));
	emp3->salary = 60000;
	strncpy(emp3->designation, "SE4", strlen("SE4"));
	emp3->emp_id = 27;	
	emp3->glnode.left = emp3->glnode.right = NULL;
	
	glthread_t *emp_list = calloc(1, sizeof(glthread_t));
	init_glthread(emp_list, OFFSETOF(emp_t, glnode));	

	glthread_add(emp_list, &emp1->glnode);
	glthread_add(emp_list, &emp2->glnode);
	glthread_add(emp_list, &emp3->glnode);

	glthread_node_t* start = emp_list->head;
	do {
		emp_t *ptr = (emp_t *)((char *)start - emp_list->offset);
		print_emp_details(ptr);
		printf("\n");
		start = start->right;
	} while(start != emp_list->head);

	while(emp_list->head != NULL) {
		emp_t *ptr = (emp_t *)((char *)emp_list->head - emp_list->offset);	
		print_emp_details(ptr);	
		printf("\n");
		glthread_remove(emp_list, &ptr->glnode);
		free(ptr);	
	}

	return 0;
}
