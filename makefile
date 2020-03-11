
all: assembler

assembler: symboltable.h symboltable.c constants.c constants.h assembler.c assembler.h firstpass.c secondpass.c main.c utils.c utils.h
	gcc -o assembler -Wall -ansi -pedantic -g symboltable.c constants.c firstpass.c secondpass.c assembler.c main.c utils.c

clean:
	rm -fr assembler test.txt assembler.zip

prepare_tests: assembler
	@mkdir -p expected
	@mkdir -p results

test1: prepare_tests
	./assembler test1.as > results/test1.log 2>&1
	@diff -q -N results/test1.log expected/test1.log || diff -s -N --side-by-side results/test1.log expected/test1.log

test2: prepare_tests
	./assembler test2.as > results/test2.log 2>&1

test: test1 test2

approve:
	rm expected/*
	cp results/* expected/

zip: test
