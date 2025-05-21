output.exe: main.o network_utils.o
	clang main.o network_utils.o -o output.exe -l pthread

network_utils.o: network_utils.h network_utils.c
	clang -g -Wall -Wextra -c network_utils.c

main.o:
	clang -g -c -Wall -Wextra main.c

clean:
	rm *.o
	rm output.exe
