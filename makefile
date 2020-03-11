
all: assembler

assembler: symboltable.h symboltable.c constants.c constants.h assembler.c assembler.h firstpass.c secondpass.c main.c utils.c utils.h
	gcc -o assembler -Wall -ansi -pedantic -g symboltable.c constants.c firstpass.c secondpass.c assembler.c main.c utils.c

clean:
	rm -fr assembler test.txt assembler.zip

test: assembler
	rm -fr test_result.txt assembler.zip
	echo "test1\n"   >> test_result.txt
	./assembler test1.as >> test_result.txt
	echo "\n"   >> test_result.txt
	echo "---------------------------------------\n"   >> test_result.txt
	echo "\n"   >> test_result.txt

zip: test
