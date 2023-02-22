COMPILER = gcc
COMPILER_FLAGS = -Wall -pedantic -g

.PHONY:

calc	:	fancy_calc.c
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

exe	:
	./calc


chat	:	chatgpt.c
		$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

exec	:
	./chat

