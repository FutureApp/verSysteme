CC=gcc
CFLAGS=-Wall -g
LDFLAGS=



all: tcp-demo-client tcp-fork-server

tcp-demo-client: tcp-demo-client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

tcp-fork-server: tcp-fork-server.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f tcp-demo-client tcp-fork-server
	rm -Rf tcp-demo-client.dSYM tcp-fork-server.dSYM

