
all: assembler

assembler: symboltable.h symboltable.c constants.c constants.h assembler.c assembler.h firstpass.c secondpass.c main.c
	gcc -o -Wall -ansi -pedantic -g symboltable.c constants.c firstpass.c secondpass.c assembler.c main.c

clean:

test: 

zip: test
