CC = gcc
CFLAGS = -Wall -pedantic -g

.PHONY: s c cl


make	:	client server



client	:	simple_client.c
	$(CC) $(CFLAGS) $^ -o $@


server	:	select_server.c
	$(CC) $(CFLAGS) $^ -o $@



s	:
	./server

c	:
	./client

cl	:
	rm *.sck