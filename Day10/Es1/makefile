CC = gcc
CFLAGS = -Wall -g -pedantic


make	:	sig_handler.c
	$(CC) $(CFLAGS) $^ -o $@

exec	:	make
	./make