all:
	$(error You must specify "generate" or a question number: "q1" or "q2" or "q3")

q1:
	gcc search_1.c -std=c99 -fsanitize=address -D_BSD_SOURCE -o search -lpthread

q2:
	gcc search_2.c -std=c99 -fsanitize=address -D_BSD_SOURCE -o search -lpthread

qvar:
	gcc search_variation.c -fsanitize=address -std=c99 -D_BSD_SOURCE -o search -lpthread

generate:
	gcc generate.c -std=c99 -o generate

clean:
	rm -f search generate
