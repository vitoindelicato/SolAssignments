CC = gcc
CFLAGS = -Wall -pedantic -g

.PHONY: s c cl


make	:	client server



client	:	client.c
	$(CC) $(CFLAGS) $^ -o $@


server	:	bc_server.c
	$(CC) $(CFLAGS) $^ -o $@



s	:
	./server

c	:
	./client

cl	:
	rm *.sck