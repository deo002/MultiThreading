output: threadbarrier.o main.o
	clang threadbarrier.o main.o -o output -l pthread

threadbarrier.o: threadbarrier.h threadbarrier.c
	clang -g -c -Wall -Wextra threadbarrier.c

main.o: main.c
	clang -g -c -Wall -Wextra main.c

clean:
	rm *.o
	rm output

