all: analyse.o shadow.o
	gcc -m32 analyse.o shadow.o -o Debug/all
	rm -f *.o
analyse.o: analyse.c
	gcc -m32 -c analyse.c -o analyse.o
shadow.o: shadow.c
	gcc -m32 -c shadow.c -o shadow.o
clean:
	rm -f *.o
	rm -f Debug/all