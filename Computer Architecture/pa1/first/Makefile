OUTPUT=first
CFLAGS=-Wall -Werror -fsanitize=address -std=c99

%: %.c %.h
	gcc $(CFLAGS) -o $@ $<
	
%: %.c
	gcc $(CFLAGS) -o $@ $<
		
all: $(OUTPUT)
		
clean:
	rm -f *.o $(OUTPUT)
