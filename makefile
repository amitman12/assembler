
all: assembler

assembler: *.h *.c
	gcc -o assembler -Wall -ansi -pedantic -g symboltable.c constants.c firstpass.c secondpass.c assembler.c main.c utils.c

clean:
	rm -fr assembler test.txt assembler.zip

prepare_tests: assembler
	@mkdir -p expected
	@mkdir -p results

test0: prepare_tests
	./assembler test0 > results/test0.log 2>&1 || true
	@ls test0.* | sort | grep -v test0.as | xargs cat >> results/test0.log
	@diff --strip-trailing-cr -q -N results/test0.log expected/test0.log || diff -s -N --side-by-side results/test0.log expected/test0.log && echo test0 O.K || true

test1: prepare_tests
	./assembler test1 > results/test1.log 2>&1 || true
	@ls test1.* | sort | grep -v test1.as | xargs cat >> results/test1.log
	@diff --strip-trailing-cr -q -N results/test1.log expected/test1.log || diff -s -N --side-by-side results/test1.log expected/test1.log && echo test1 O.K || true

test2: prepare_tests
	./assembler test2 > results/test2.log 2>&1 || true
	@ls test2.* | sort | grep -v test2.as | xargs cat >> results/test2.log
	@diff --strip-trailing-cr -q -N results/test2.log expected/test2.log || diff -s -N --side-by-side results/test2.log expected/test2.log && echo test2 O.K || true

test3: prepare_tests
	./assembler test3 > results/test3.log 2>&1 || true
	@ls test3.* | sort | grep -v test3.as | xargs cat >> results/test3.log
	@diff --strip-trailing-cr -q -N results/test3.log expected/test3.log || diff -s -N --side-by-side results/test3.log expected/test3.log && echo test3 O.K || true

test4: prepare_tests
	./assembler test4 > results/test4.log 2>&1 || true
	@ls test4.* | sort | grep -v test4.as | xargs cat >> results/test4.log
	@diff --strip-trailing-cr -q -N results/test4.log expected/test4.log || diff -s -N --side-by-side results/test4.log expected/test4.log && echo test4 O.K || true

test5: prepare_tests
	./assembler test5 > results/test5.log 2>&1 || true
	@ls test5.* | sort | grep -v test5.as | xargs cat >> results/test5.log
	@diff --strip-trailing-cr -q -N results/test5.log expected/test5.log || diff -s -N --side-by-side results/test5.log expected/test5.log && echo test5 O.K || true

test6: prepare_tests
	./assembler test6 > results/test6.log 2>&1 || true
	@ls test6.* | sort | grep -v test6.as | xargs cat >> results/test6.log
	@diff --strip-trailing-cr -q -N results/test6.log expected/test6.log || diff -s -N --side-by-side results/test6.log expected/test6.log && echo test6 O.K || true

test7: prepare_tests
	./assembler test7 > results/test7.log 2>&1 || true
	@ls test7.* | sort | grep -v test7.as | xargs cat >> results/test7.log
	@diff --strip-trailing-cr -q -N results/test7.log expected/test7.log || diff -s -N --side-by-side results/test7.log expected/test7.log && echo test7 O.K || true

test8: prepare_tests
	./assembler test8 > results/test8.log 2>&1 || true
	@ls test8.* | sort | grep -v test8.as | xargs cat >> results/test8.log
	@diff --strip-trailing-cr -q -N results/test8.log expected/test8.log || diff -s -N --side-by-side results/test8.log expected/test8.log && echo test8 O.K || true

test9: prepare_tests
	./assembler test9 > results/test9.log 2>&1 || true
	@ls test9.* | sort | grep -v test9.as | xargs cat >> results/test9.log
	@diff --strip-trailing-cr -q -N results/test9.log expected/test9.log || diff -s -N --side-by-side results/test9.log expected/test9.log && echo test9 O.K || true

test10: prepare_tests
	./assembler test10 > results/test10.log 2>&1 || true
	@ls test10.* | sort | grep -v test10.as | xargs cat >> results/test10.log
	@diff --strip-trailing-cr -q -N results/test10.log expected/test10.log || diff -s -N --side-by-side results/test10.log expected/test10.log && echo test10 O.K || true

test_multi: prepare_tests
	./assembler test_multi1 test_multi2 test_multi3 > results/test_multi.log 2>&1 || true
	@ls test_multi* | sort | grep -v 'test.*as' | while read filename; do (echo ""; echo "file:$$filename"; echo "=============="; cat "$$filename") >> results/test_multi.log; done
	@diff --strip-trailing-cr -q -N results/test_multi.log expected/test_multi.log || diff -s -N --side-by-side results/test_multi.log expected/test_multi.log && echo test_multi O.K || true


test: test0 test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test_multi

approve: prepare_tests
	cp results/* expected/

zip: test
	zip assembler.zip -r assembler readme.md *.c *.h makefile test*.as expected

