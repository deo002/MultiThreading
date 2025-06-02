
output: main.o sema.o
	clang main.o sema.o -o output -l pthread

sema.o: sema.c sema.h
	clang -g -c -Wall -Wextra sema.c

main.o: main.c
	clang -g -c -Wall -Wextra main.c


