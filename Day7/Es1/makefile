CC = gcc
CFLAGS = -Wall -pedantic -g -pthread
FLAGS = -lpthread
.PHONY: all

#gcc -g -Wall -pthread yourcode.c -lpthread -o yourprogram

# $@    :    nome del target
# $^    :    dependency list
# $<    :    first name of dependency list



prod_cons   :    prod_cons.c
	$(CC) $(CFLAGS) $^ $(FLAGS)  -o $@

exe	:
	./prod_cons