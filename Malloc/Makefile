all: memgrind

mymalloc: mymalloc.c
	gcc -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c
	gcc -c mymalloc.c
	gcc -o memgrind memgrind.c mymalloc.o
clean:
	rm -f mymalloc.o memgrind