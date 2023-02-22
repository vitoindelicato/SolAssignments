CC = gcc
CFLAGS = -Wall -pedantic
.PHONY: all

# $@    :    nome del target
# $^    :    dependency list
# $<    :    first name of dependency list



zombieficator    :    zombieficator.c
	$(CC) $(CFLAGS) $^ -o $@

all	:
	sleepy

exe	:
	./zombieficator 5 &