all: analyse.o alloc.o shadow.o error.o check.o
	gcc -g -m32 analyse.o alloc.o shadow.o error.o check.o -o Debug/all
	rm -f *.o
analyse.o: analyse.c
	gcc -g -m32 -c analyse.c -o analyse.o
shadow.o: shadow.c
	gcc -g -m32 -c shadow.c -o shadow.o
alloc.o: alloc.c
	gcc -g -m32 -c alloc.c -o alloc.o
error.o: error.c
	gcc -g -m32 -c error.c -o error.o
check.o: check.c
	gcc -g -m32 -c check.c -o check.o
clean:
	rm -f *.o
	rm -f Debug/all