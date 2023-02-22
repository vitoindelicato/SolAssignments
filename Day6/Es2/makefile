CC = gcc
CFLAGS = -Wall -pedantic
.PHONY: all

# $@    :    nome del target
# $^    :    dependency list
# $<    :    first name of dependency list



sleepy    :    sleepy_son.c
	$(CC) $(CFLAGS) $^ -o $@

all	:
	sleepy

exe	:
	./sleepy -s 5

clean    :
	@echo "Removing garbage"
	-rm -f *.o
	-rm -f *.txt
	-rm -f *.dat