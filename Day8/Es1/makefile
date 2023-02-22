COMPILER = gcc
COMPILER_FLAGS = -Wall -pedantic -g

.PHONY:

queue	:	infinity_queue.c
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

exe	:
	./queue 10

valgrind	:
	valgrind ./queue 20