all: memgrind.o mymalloc.o
	gcc memgrind.o mymalloc.o mymalloc.h -g -o memgrind

memgrind.o: memgrind.c mymalloc.h
	gcc -std=c99 -c -g memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	gcc -std=c99 -c -g mymalloc.c

r: all
	./memgrind

debug:
	gdb ./memgrind

clean:
	rm memgrind memgrind.o mymalloc.o
