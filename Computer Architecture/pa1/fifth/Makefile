OUTPUT=fifth
CFLAGS=-Wall -Werror -std=c99

%: %.c %.h
	gcc $(CFLAGS) -o $@ $<
	
%: %.c
	gcc $(CFLAGS) -o $@ $<
		
all: $(OUTPUT)
		
clean:
	rm -f *.o $(OUTPUT)
