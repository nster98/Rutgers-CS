all:
	@echo "ERROR: Please compile with the arguments \"proc\" or \"thread\""

proc: searchtest.o multitest_proc.o
	gcc searchtest.o multitest_proc.o multitest.h -g -lm -o searchtest

thread: searchtest.o multitest_thread.o
	gcc searchtest.o multitest_thread.o multitest.h -g -lpthread -lm -std=c99 -o searchtest

multitest_proc.o: multitest_proc.c multitest.h
	gcc -std=c99 -c -g  multitest_proc.c

multitest_thread.o: multitest_thread.c multitest.h
	gcc -std=c99 -lpthread -c -g multitest_thread.c

searchtest.o: searchtest.c multitest.h
	gcc -std=c99 -c -g searchtest.c

run: 
	./searchtest

debug:
	gdb ./searchtest

clean:
	rm *.o searchtest

