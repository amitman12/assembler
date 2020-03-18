
all: assembler

assembler: *.h *.c
	gcc -o assembler -Wall -ansi -pedantic -g symboltable.c constants.c firstpass.c secondpass.c assembler.c main.c utils.c

clean:
	rm -fr assembler test.txt assembler.zip

prepare_tests: assembler
	@mkdir -p expected
	@mkdir -p results

test1: prepare_tests
	./assembler test1.as > results/test1.log 2>&1 || /bin/true
	@diff -q -N results/test1.log expected/test1.log || diff -s -N --side-by-side results/test1.log expected/test1.log && echo test1 O.K

test2: prepare_tests
	./assembler test2.as > results/test2.log 2>&1 || /bin/true
	@diff -q -N results/test2.log expected/test2.log || diff -s -N --side-by-side results/test2.log expected/test2.log && echo test2 O.K

test3: prepare_tests
	./assembler test3.as > results/test3.log 2>&1 || /bin/true
	@diff -q -N results/test3.log expected/test3.log || diff -s -N --side-by-side results/test3.log expected/test3.log && echo test3 O.K

test4: prepare_tests
	./assembler test4.as > results/test4.log 2>&1 || /bin/true
	@diff -q -N results/test4.log expected/test4.log || diff -s -N --side-by-side results/test4.log expected/test4.log && echo test4 O.K

test5: prepare_tests
	./assembler test5.as > results/test5.log 2>&1 || /bin/true
	@diff -q -N results/test5.log expected/test5.log || diff -s -N --side-by-side results/test5.log expected/test5.log && echo test5 O.K

test6: prepare_tests
	./assembler test6.as > results/test6.log 2>&1 || /bin/true
	@diff -q -N results/test6.log expected/test6.log || diff -s -N --side-by-side results/test6.log expected/test6.log && echo test6 O.K

test7: prepare_tests
	./assembler test7.as > results/test7.log 2>&1 || /bin/true
	@diff -q -N results/test7.log expected/test7.log || diff -s -N --side-by-side results/test7.log expected/test7.log && echo test7 O.K

test8: prepare_tests
	./assembler test8.as > results/test8.log 2>&1 || /bin/true
	@diff -q -N results/test8.log expected/test8.log || diff -s -N --side-by-side results/test8.log expected/test8.log && echo test8 O.K

test9: prepare_tests
	./assembler test9.as > results/test9.log 2>&1 || /bin/true
	@diff -q -N results/test9.log expected/test9.log || diff -s -N --side-by-side results/test9.log expected/test9.log && echo test9 O.K

test10: prepare_tests
	./assembler test10.as > results/test10.log 2>&1 || /bin/true
	@diff -q -N results/test10.log expected/test10.log || diff -s -N --side-by-side results/test10.log expected/test10.log && echo test10 O.K






test: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10

approve: prepare_tests
	cp results/* expected/

zip: test
