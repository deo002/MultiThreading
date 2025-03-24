#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OFFSET_OF(type, field) (size_t)&(((type *)0)->field)

typedef struct _emp {
	char name[30];
	unsigned int salary;
	char designation[30];
	unsigned int emp_id;
} emp_t;

void print_emp_detail(unsigned int * empid) {
	size_t offset = OFFSET_OF(emp_t, emp_id);
	
	/* turning pointer into type char *. If I have an int * pointer
	then in pointer arithmetic incremeting by x will lead it to +4*x bytes
	and not 1. It moves in multiples of size of data structure it points to  */
	emp_t *base = (emp_t *)((char *)empid - offset);

	printf("Name: %s\n", base->name);
	printf("Salary: %d", base->salary);
}

int main(int rc, char **argv) {
	emp_t *emp = calloc(1, sizeof(emp_t));
	strcpy(emp->name, "Dearsh");
	emp->salary = 1500000;
	strcpy(emp->designation, "Engineer");
	emp->emp_id = 1;

	print_emp_detail(&emp->emp_id);

	free(emp);
	emp = NULL;	

	return 0;
}
