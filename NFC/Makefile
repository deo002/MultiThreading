output: notif.o glthreads.o main.o ip.o subscriber.o
	clang main.o notif.o glthreads.o ip.o subscriber.o -o output -l pthread

notif.o: notif.c notif.h
	clang -g -c -Wall -Wextra notif.c

glthreads.o: ../GLThreads/glthreads.c ../GLThreads/glthreads.h
	clang -g -c -Wall -Wextra  ../GLThreads/glthreads.c

ip.o: ip.c ip.h
	clang -g -c -Wall -Wextra ip.c

main.o: main.c
	clang -g -c -Wall -Wextra main.c

subscriber.o: subscriber.c
	clang -g -c -Wall -Wextra subscriber.c

clean:
	rm *.o
	rm output
