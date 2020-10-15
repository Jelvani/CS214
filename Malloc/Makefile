CC = gcc

memgrind: mymalloc.o memgrind.o
	$(CC) memgrind.o mymalloc.o -o memgrind
memgrind.o: memgrind.c mymalloc.h
	$(CC) -c memgrind.c
mymalloc.o: mymalloc.c mymalloc.h
	$(CC) -c mymalloc.c
clean:
	rm -f memgrind *.o
