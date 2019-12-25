all: client serve

client: DUMBclient.c
	gcc DUMBclient.c -g -std=c99 -o DUMBclient

serve: DUMBserver.c
	gcc DUMBserver.c -g -std=c99 -lpthread -o DUMBserve

launchServer: 
	./DUMBserve 23490

launchClient: 
	./DUMBclient kill.cs.rutgers.edu 23490

s: launchServer

c: launchClient

clean:
	rm DUMBserve DUMBclient
