CC=gcc
CFLAGS=-Wall -g
LDFLAGS=



all: tcp-demo-client tcp-demo-server

tcp-demo-client: tcp-demo-client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

tcp-demo-server: tcp-demo-server.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f tcp-demo-client tcp-demo-server
	rm -Rf tcp-demo-client.dSYM tcp-demo-server.dSYM

