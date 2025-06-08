output: crud.o refcount.o student_list.o glthreads.o
	clang crud.o refcount.o student_list.o glthreads.o -o output -l pthread

glthreads.o: ../GLThreads/glthreads.c ../GLThreads/glthreads.h
	clang -g -c -Wall -Wextra  ../GLThreads/glthreads.c

student_list.o: student_list.h student_list.c
	clang -g -c -Wall -Wextra student_list.c

refcount.o: refcount.h refcount.c
	clang -g -c -Wall -Wextra refcount.c

crud.o: crud.c
	clang -g -c -Wall -Wextra crud.c

clean:
	rm *.o
	rm output

