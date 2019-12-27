
all: assembler

assembler: symboltable.c constants.c
	gcc -o get_name -Wall -ansi -pedantic -g symboltable.c constants.c

clean:

test: get_name

zip: test
