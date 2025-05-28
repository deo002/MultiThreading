output.exe: main.o queue.o
	clang main.o queue.o -o output.exe

queue.o: queue.h queue.c
	clang -g -c -Wall -Wextra queue.c

main.o: main.c
	clang -g -c -Wall -Wextra main.c

clean:
	rm output.exe
	rm *.o
