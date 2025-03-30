output: main.o glthreads.o
	clang main.o glthreads.o -o output

main.o: main.c
	clang -g -c main.c

glthreads.o: glthreads.c glthreads.h
	clang -g -c glthreads.c

clean:
	rm *.o output
