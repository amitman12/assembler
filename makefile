
all: assembler

assembler: symboltable.h symboltable.c constants.c constants.h assembler.c assembler.h firstpass.c secondpass.c main.c utils.c utils.h
	gcc -o -Wall -ansi -pedantic -g symboltable.c constants.c firstpass.c secondpass.c assembler.c main.c utils.c

clean:

test: 

zip: test
