CC = gcc
CFLAGS = -Wall -pedantic


# $@    :    nome del target
# $^    :    dependency list
# $<    :    first name of dependency list


.PHONY: exe


turns :	my_your_turns.c
	$(CC) $(CFLAGS) $^ -o  $@

exe :
	./turns


