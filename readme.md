some important points:

1. there are four types of sentences as defined in page 23.
	according to the explanation, an empty cannot have a label definition.
	therefore, a sentence like   SOMELABEL:   
	is illegal as it is not an empty sentence, nor is it a command/derictive 	 sentence.

2. in page 25, a "legal label" is defined:
	a legal label starts with an alphabetic letter followed by either letters
	or numbers so that the length is 31 at most, not including \0.
	then, immediately after the last character of the label, there is a ':'.
	whitespaces are not numbers or letters.
	therefore, there cannot be any whitespaces in between the label and the :
	as in, LabelExample   : add r5,r6 is illegal.
	eventhough later in the explanation it is demanded to allow any amount
	of whitespaces - it was not clear which rule to follow here...

3. the errors throwed by this program don't mention the commas.
	if too many/too little commas are given, the program will fail to
	recognize the operand/s and the error will be simillar to:
	too many/little/illegal operands for command/directive line ... etc
	------

4. GENERAL ALGORITHM:
	the questions in the website showed that the algorithm in page 33 is
	only a suggested one, as in, it can be changed and tweaked.
	therefore unlike the algorithm, firstPass does NOT have anything to do
	with coding command lines. it does not code ANY command lines, even
	if they do not depend on labels.. etc.
	firstPass DOES, however, code directive lines and stores that in 		allocated space. the rest is like the given algorithm - firstPass updates
	a data structure for labels in code.
	after secondPass is over, the data copied into the allocated space in 		firstPass will then be copied into output files, AFTER the command lines 	 have been coded. (and assuming there are no errors);
	the translation of given lines into binary and then octal happends 		during secondPass. the output is stored in an internal array, and once 		we run out of code, if there are no errors, output files are created,
	the translated command lines are copies, and then the .data/.string
	lines are added below the code segment.

5. there are some command combinations that seem strange, for example:
	it was not clear if this is legal or not:
	jmp K
	END: stop
	K: .data 31
	jumping to a directive line. no stop, etc..
	my program ALLOWS such cases as it was not specified whether it should 		do so or not.

6. I have created a few test files that contain legal and illegal scenarios.
	for each one of these files, I also created the expected results
	each output (for testfile) is compared to the expected output and 
	OK is printed if the test succeeded.
	test0 is exactly the code in page 37.
	
