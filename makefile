all: analyse.o alloc.o shadow.o error.o
	gcc -m32 analyse.o alloc.o shadow.o error.o -o Debug/all
	rm -f *.o
analyse.o: analyse.c
	gcc -m32 -c analyse.c -o analyse.o
shadow.o: shadow.c
	gcc -m32 -c shadow.c -o shadow.o
alloc.o: alloc.c
	gcc -m32 -c alloc.c -o alloc.o
error.o: error.c
	gcc -m32 -c error.c -o error.o
clean:
	rm -f *.o
	rm -f Debug/all