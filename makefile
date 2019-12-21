
all: get_name

get_name: get_name.c
	gcc -o get_name -Wall -ansi -pedantic -g get_name.c

clean:
	rm -fr get_name test.log get_name.zip

test: get_name
	cat test_names_1 | ./get_name >> test.log
	echo "\n\n\n\n\n"   >> test.log
	-cat test_names_2 | ./get_name >> test.log
	echo "\n\n\n\n\n"   >> test.log
	cat test_names_3 | ./get_name >> test.log
	echo "\n\n\n\n\n"   >> test.log	
	-cat test_names_4 | ./get_name >> test.log
	echo "\n\n\n\n\n"   >> test.log
zip: test
	zip get_name.zip get_name get_name.c test.log makefile get_name.h test_names_1 test_names_2 test_names_3 test_names_4
