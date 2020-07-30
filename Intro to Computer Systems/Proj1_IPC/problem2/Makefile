all:
	$(error You must specify "generate" or a question number: "q1" or "q2" or "q3")

q1:
	gcc search_1.c -std=c99 -D_BSD_SOURCE -o search

q2:
	gcc search_2.c -std=c99 -D_BSD_SOURCE -o search

q3:
	gcc search_3.c -std=c99 -D_BSD_SOURCE -o search

qvar:
	gcc search_variation.c -std=c99 -D_BSD_SOURCE -o search

generate:
	gcc generate.c -std=c99 -o generate

clean:
	rm -f search generate
