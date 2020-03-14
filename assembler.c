#include <stdio.h>
#include <ctype.h>
#include "symboltable.h"
#include "assembler.h"
#include "utils.h"
#include "constants.h"
#include "firstpass.h"




int codeString(struct assemblerContext* context, char* str){
	/* code string and put it where it should be */
	/* we need a word per letter in str, so 2bytes per letter */
	context->memory = (char*)realloc(context->memory,(strlen(str)+1)*BYTES_IN_WORD);
	if(context->memory==NULL){
		fprintf(stderr,"ERROR: could not allocate memory\n");
	}
	memcpySTRING(((context->memory)+(context->dataCount)),str,strlen(str)+1);
	/* code words and copy them to allocated memory */
	return 1;
}

void memcpyNUM(char* ptr,int* num){
	/* first character gets zero, then copies second character*/
	*ptr = 0;
	memcpy(ptr+1,num,1);
}

int codeNumber(struct assemblerContext* context, int number, int count){
	/* reallocates space for .data argument and writes it in memory in correct base */
	/* returns 0 on success*/
	context->memory = (char*)realloc(context->memory,BYTES_IN_WORD);
	/* each word in machine code is 15 bits which means we need 2 bytes per word. and a number is 1 word  */
	if(context->memory==NULL){
		fprintf(stderr,"ERROR: could not allocate memory\n");
		free(context->memory);
		return -1;
	}
	/* we write into memory in location data count + count */
	memcpyNUM(((context->memory)+(context->dataCount + count)),&number);
	/* code words and copy them to allocated memory */
	return 0;
}




int processDotEntry(struct assemblerContext* context, char* directive, char* args, int pass){
	return 1;
}


int processDotExtern(struct assemblerContext* context, char* directive, char* args,int pass){
	/* args points to first argument */
	/* returns errors or number of machine code words needed*/
	/* adds extern labels to symboltable */
	char* label =(char*)calloc(MAX_LABEL,sizeof(char));
	int count = 0;
	if(pass==SECOND_PASS){
		/* code directive and put it where it should be */
	}
	while(*args!='\0'){
		args = readSymbol(args,label);
		if(args==NULL){
			fprintf(stderr, "%s:%d: ERROR: invalid argument for .extern \n",context->fileName,context->lineNumber);
			free(label);
			return ILLEGAL_LABEL;
		}
		if(is_reserved_word(label)==1){
			fprintf(stderr, "%s:%d: ERROR: argument for .extern can't be a reserved word \n",context->fileName,context->lineNumber);
			free(label);
			return ILLEGAL_LABEL;
		}
		if(is_legal(label)==-1){
			fprintf(stderr, "%s:%d: ERROR: illegal label argument for .extern \n",context->fileName,context->lineNumber);
			free(label);
			return ILLEGAL_LABEL;
		}
		/*if(find_symbol(&context->table,label)!=NULL){
			fprintf(stderr, "%s:%d: ERROR: illegal label argument for .extern \n",context->fileName,context->lineNumber);
			free(label);
			return LABEL_ALREADY_EXISTS;
		}*/
		/* we don't check if a label given as argument in .extern line already exists in symboltable*/
		if(pass==FIRST_PASS){
			add_symbol(&context->table, create_symbol(&context->table, label, UNKNOWN_ADDRESS, External,
					Unknown_Location));
		}
		else{
			/* second pass*/
			/* code label and put it where it's supposed to be */
		}

		count++;
		if(*args==','){
			++args;
			args = skipWhiteSpaces(args);
		}
	}
	if(count==0){
		fprintf(stderr, "%s:%d: ERROR: no arguments for .extern line \n",context->fileName,context->lineNumber);
		free(label);
		return LABEL_EMPTY;
	}
	/* return number of extern labels in this line */
	free(label);
	return count;
}

int processDotData(struct assemblerContext* context, char* args, int pass){
	/* args points to first argument */
	/* returns errors or number of words needed in machine code for this .string line  on success */
	int count = 0;
	int current_immediate=0;
	int* temp = &current_immediate;
	int err = 0;
	int* err_type = &err;

	if(args=='\0'){
		fprintf(stderr, "%s:%d: ERROR: .data directive can't have an empty immediate \n",context->fileName,context->lineNumber);
		return -1;
	}
	while(*args!='\0'){
		args = readNumber(args,temp,err_type);
		if(*err_type==EMPTY_IMMEDIATE){
			fprintf(stderr, "%s:%d: ERROR: .data directive wrong operand type or empty immediate \n",context->fileName,context->lineNumber);
			return -1;
		}
		if(*err_type==SYNTAX_ERROR){
			fprintf(stderr, "%s:%d: ERROR: .data directive syntax error\n",context->fileName,context->lineNumber);
			return -1;
		}
		/* immediate is in *temp */
		if(pass==FIRST_PASS){
			codeNumber(context,*temp,count);
		}
		count++;
		if(*args==','){
			args++;
			args = skipWhiteSpaces(args);
			/* now points to next number*/
		}
	}
	/* we need 1 word per immediate plus one for the .data directive*/
	return count+1;
}

int processDotString(struct assemblerContext* context, char* args, int pass){
	/* args points to first argument */
	/* returns errors or number of words needed in machine code for this .string line  on success */
	char str[MAX_CMD];
	int* len;
	len =(int*)malloc(sizeof(int));
	if(len==NULL){
		fprintf(stderr, "Error: memory allocation failed");

	}
	readString(args,str,len);
	if(*len==EMPTY_STRING){
		free(len);
		fprintf(stderr, "%s:%d: ERROR: .string directive can't have an empty string\n",context->fileName,context->lineNumber);
		return -1;

	}
	if(*len == SYNTAX_ERROR){
		free(len);
		fprintf(stderr, "%s:%d: ERROR: .string directive syntax error\n",context->fileName,context->lineNumber);
		return -1;

	}
	/* string is in str*/
	if(pass==FIRST_PASS){
		codeString(context->memory,str);
	}
	/* we need one word of machine code per char in the string(including \0) plus one for the command */
	return *len+1;
}





struct commandInfo* findCommand(char* cmd) {
	/*return pointer to commandInfo if found or NULL if not*/
	struct commandInfo* c;
	for (c = commands; c->command != NULL; ++c) {
		if (strcmp(c->command, cmd) == 0) {
			return c;
		}
	}
	return NULL;
}

char* readCommandOperand(struct assemblerContext* context, char* p, struct operand* op, int isCommand) {
	/* puts addressing in op */
	/* p points to first char in operand (isn't whitespace)*/
	/* returns pointer to first char in the next operand */
	/* is command is 1 if line is a command line and 0 on directive line */
	int* err_type;
	char* nextToken;
	err_type = (int*)malloc(sizeof(int));
	op->addressingType = 0;

	if (*p == '*'&& isCommand==1) {
		p++;
		nextToken = readRegister(p, &(op->info.registerId));
		if (nextToken == NULL) {
			free(err_type);
			if(op->info.registerId==INVALID_REGISTER){
				fprintf(stderr, "%s:%d: ERROR: expecting a valid register name\n",context->fileName,context->lineNumber);
			}
			return NULL;
		}
		op->addressingType = Indirect_Register_Addressing;
		op->operandType = Register;

	} else if (*p == '#'&& isCommand==1) {
		p++;
		nextToken = readNumber(p, &op->info.immediateValue,err_type);
		if (*err_type == SYNTAX_ERROR) {
			free(err_type);
			fprintf(stderr,"%s:%d: ERROR: expecting a valid immediate\n",context->fileName,context->lineNumber);
			return NULL;
		}
		else if(*err_type == EMPTY_IMMEDIATE){
			free(err_type);
			fprintf(stderr,"%s:%d: ERROR: immediate can't be empty\n",context->fileName,context->lineNumber);
			return NULL;
		}
		free(err_type);
		op->addressingType = Immediate_Addressing;
		op->operandType = Immediate;

	} else if(isCommand==1) {
		nextToken = readRegister(p, &(op->info.registerId));
		if (nextToken != NULL) {
			free(err_type);
			op->addressingType = Direct_Register_Addressing;
			op->operandType = Register;
		} else {
            nextToken = readSymbol(p, (op->info.symbolName));
            if (nextToken != NULL) {
        		free(err_type);
                op->addressingType = Direct_Addressing;
                op->operandType = Label;

            }
    		else if(isCommand==0){
    			nextToken = readNumber(p,&(op->info.immediateValue),err_type);
    			if(nextToken != NULL){
                    op->addressingType = Direct_Addressing;
                    op->operandType = Immediate;
    				free(err_type);
    			}
    		}
		}


		if (nextToken == NULL) {
			free(err_type);
            fprintf(stderr,"%s:%d: ERROR: expected valid operand\n",context->fileName,context->lineNumber);
            return NULL;
		}
	}
	return nextToken;
}

/*group 1 commands: mov,add,sub */
/*group 2 commands: cmp */
/*group 3 commands: lea */
/*group 4 commands: clr, not, inc, dec, red*/
/*group 5 commands: jmp, jsr, bne*/
/*group 6 commands: prn */
/*group 7 commands: rts, stop*/

int processGroup1Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	/*commands of group 1 - mov,add,sub */
	/* args points to first char of first argument in line */
	char* p;
	char* nextToken;
	struct operand op1;
	struct operand op2;

	p = skipWhiteSpaces(args);
	/* p points to the first non whitespace character in args */
	nextToken = readCommandOperand(context, p, &op1,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	nextToken = skipWhiteSpaces(nextToken);
	if (*nextToken != ',') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	nextToken++;
	nextToken = skipWhiteSpaces(nextToken);

	nextToken = readCommandOperand(context, nextToken, &op2,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	p = skipWhiteSpaces(nextToken);
	if (*p != '\0') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	/* both operands were parsed successfully*/
	if(pass==FIRST_PASS){
		if ((op1.addressingType == 2 || op1.addressingType == 3)
				&& (op2.addressingType == 2 || op2.addressingType == 3)) {
			/* we only need one extra word in machine code for both operands */
			return 2;
		} else if (op2.addressingType == 0) {
			fprintf(stderr,"%s:%d: ERROR: dest operand addressing of command %s cannot be immediate addressing\n",context->fileName, context->lineNumber, cmd);
			return ADDRESSING_ERROR;
		} else {
			/* we need one word in machine code per operand and one for the command */
			return 3;
		}
	}

	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1 and operand 2 is in op2 */
	return 1;


}

int processGroup2Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	/* command of group 2 - cmp */
	char* p;
	char* nextToken;
	struct operand op1;
	struct operand op2;

	p = skipWhiteSpaces(args);
	/* p points to the first non whitespace character in args */
	nextToken = readCommandOperand(context, p, &op1,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	nextToken = skipWhiteSpaces(nextToken);
	if (*nextToken != ',') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	++nextToken;
	nextToken = skipWhiteSpaces(nextToken);

	nextToken = readCommandOperand(context, nextToken, &op2,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	nextToken = skipWhiteSpaces(nextToken);
	if (*nextToken != '\0') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires exactly two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	/* both operands were parsed successfully*/
	if(pass==FIRST_PASS){
		if ((op1.addressingType == 2 || op1.addressingType == 3)
				&& (op2.addressingType == 2 || op2.addressingType == 3)) {
			/* we only need one extra word in machine code for both operands */
			return 2;
		} else {
			/* all addressing methods are legal for this command */
			/* we need one word in machine code per operand and one for the command */
			return 3;
		}
	}
	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1 and operand 2 is in op2 */
	return 1;

}

int processGroup3Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	/*commands of group 3 - lea */
	/* args points to first char of first argument in line */
	char* p;
	char* nextToken;
	struct operand op1;
	struct operand op2;

	p = skipWhiteSpaces(args);
	/* p points to the first non whitespace character in args */
	nextToken = readCommandOperand(context, p, &op1,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	nextToken = skipWhiteSpaces(nextToken);
	if (*nextToken != ',') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	++nextToken;
	nextToken = skipWhiteSpaces(nextToken);


	nextToken = readCommandOperand(context, nextToken, &op2,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	nextToken = skipWhiteSpaces(nextToken);
	if (*nextToken != '\0') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires two operands\n",context->fileName, context->lineNumber, cmd);
		return -1;
	}
	/* both operands were parsed successfully*/
	if(pass==1){
		if (op1.addressingType != 1) {
			fprintf(stderr,"%s:%d: ERROR: source addressing type of command %s has to be Direct Addressing\n",context->fileName, context->lineNumber, cmd);
			return ADDRESSING_ERROR;
		} else {
			if(op2.addressingType==0){
				fprintf(stderr,"%s:%d: ERROR: target addressing type of command %s cannot be Immediate Addressing\n",context->fileName, context->lineNumber, cmd);
				return ADDRESSING_ERROR;
			}
			/* we need one word in machine code per operand and one for the command */
			return 3;
		}
	}
	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1 and operand 2 is in op2 */
	return 1;
}

int processGroup4Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	/* group 4 commands - clr, not, inc, dec, red */
	/* these commands only take 1 operand */
	char* p;
	char* nextToken;
	struct operand op;

	p = skipWhiteSpaces(args);
	/* p points to the first non whitespace character in args */
	nextToken = readCommandOperand(context, p, &op,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s requires exactly one operand\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}
	p = skipWhiteSpaces(nextToken);
	if (*p != '\0') {
		fprintf(stderr,"%s:%d: ERROR: command %s requires exactly one operand\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}

	/* operand was parsed successfully*/
	if(pass==FIRST_PASS){
		if (op.addressingType == 0) {
			fprintf(stderr,"%s:%d: ERROR: dest addressing type of command %s cannot be Immediate Addressing\n",context->fileName, context->lineNumber, cmd);
			return ADDRESSING_ERROR;
		} else {
			/* we need one word in machine code per operand and one for the command */
			/* one operand , one word*/
			return 2;
		}
	}
	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1 and operand 2 is in op2 */
	return 1;
}

int processGroup5Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	/* group 5 commands - jmp, jsr, bne*/
	/* these commands only take 1 operand */
	char* p;
	char* nextToken;
	struct operand op;

	p = skipWhiteSpaces(args);
	/* p points to the first non whitespace character in args */
	nextToken = readCommandOperand(context, p, &op,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR:command %s requires exactly one operand\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}
	p = skipWhiteSpaces(nextToken);
	if (*p != '\0') {
		fprintf(stderr,"%s:%d: ERROR:command %s requires exactly one operand\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}

	/* operand was parsed successfully*/
	if(pass==FIRST_PASS){
		if (op.addressingType == 0 || op.addressingType == 3) {
			fprintf(stderr,
					"%s:%d: ERROR: dest addressing type cannot be Immediate Addressing or Direct Register Addressing\n",context->fileName, context->lineNumber);
			return ADDRESSING_ERROR;
		} else {
			/* we need one word in machine code per operand and one for the command */
			/* one operand , one word*/
			return 2;
		}
	}
	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1*/
	return 1;
}

int processGroup6Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	/* group 6 commands - prn*/
	/* this command only takes 1 operand */
	char* p;
	char* nextToken;
	struct operand op;

	p = skipWhiteSpaces(args);
	/* p points to the first non whitespace character in args */
	nextToken = readCommandOperand(context, p, &op,1);
	if (nextToken == NULL) {
		fprintf(stderr,"%s:%d: ERROR: command %s takes exactly 1 operand\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}
	p = skipWhiteSpaces(nextToken);
	if (*p != '\0') {
		fprintf(stderr,"%s:%d: ERROR: command %s takes exactly 1 operand\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}

	/* operand was parsed successfully*/
	/* every addressing method is legal for prn */
	/* we need one word in machine code per operand and one for the command */
	/* one operand , one word*/
	if(pass==FIRST_PASS){
		return 2;
	}
	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1*/
	return 1;
}

int processGroup7Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass) {
	char* p;
	p = skipWhiteSpaces(args);
	if (*p != '\0') {
		fprintf(stderr,"%s:%d: ERROR: command %s does not accept arguments\n",context->fileName, context->lineNumber, cmd);
		return SYNTAX_ERROR;
	}
	/* one word in machine code needed*/
	if(pass==FIRST_PASS){
		return 1;
	}
	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */	/* if we get here we're in 2nd pass */
	/* successful parse and second parse - we code here */
	/* command is in cmd. operand 1 is in op1*/
	return 1;
	/* command is in cmd. operand 1 is in op1*/
	return 1;
}

struct commandInfo commands[] = { /**/
{ "mov", processGroup1Command }, /**/
{ "add", processGroup1Command }, /**/
{ "sub", processGroup1Command }, /**/
{ "cmp", processGroup2Command }, /**/
{ "lea", processGroup3Command }, /**/
{ "clr", processGroup4Command }, /**/
{ "not", processGroup4Command }, /**/
{ "inc", processGroup4Command }, /**/
{ "dec", processGroup4Command }, /**/
{ "red", processGroup4Command }, /**/
{ "jmp", processGroup5Command }, /**/
{ "jsr", processGroup5Command }, /**/
{ "bne", processGroup5Command }, /**/
{ "prn", processGroup6Command }, /**/
{ "rts", processGroup7Command }, /**/
{ "stop", processGroup7Command }, /**/
{ NULL, NULL } /**/
};

int processLine(struct assemblerContext* context, char* line, int pass) {
	/* returns number of machine code words needed for this line on successful parse */
	/* returns negatives on errors */
	/* pass = 1 on firstpass. pass = 2 on secondpass */
	int words;
	char* p = line;
	char* nextToken;
	char label[MAX_LABEL];
	char cmd[MAX_CMD];
	int result;
	int labelFlag = 0;
	struct commandInfo* cmdInfo;
	p = skipWhiteSpaces(p);
	if (*p == ';' || *p == '\0') {
		/* comment line*/
		return 0;
	}
	/* we assume that after a label name there cannot be spaces */
	nextToken = readToken(p, " \t:");
	if (*nextToken == ':') {
		/* new label defined in this line*/
		if (nextToken - p == 0) {
			fprintf(stderr,"%s:%d: ERROR: expecting non empty label\n", context->fileName, context->lineNumber);
			return LABEL_EMPTY;
		}
		if (nextToken - p > MAX_LABEL) {
			fprintf(stderr,"%s:%d: ERROR: label too long\n", context->fileName, context->lineNumber);
			return LABEL_TOO_LONG;
		}
		strncpyNull(label, p, nextToken - p);
		if (find_symbol(&context->table, label)) {
			fprintf(stderr,"%s:%d: ERROR: label already exists\n", context->fileName, context->lineNumber);
			return LABEL_ALREADY_EXISTS;
		}
		if (is_reserved_word(label)) {
			fprintf(stderr,"%s:%d: ERROR: label can't be a reserved word\n", context->fileName, context->lineNumber);
			return RESERVED_WORD;
		}
		if (is_legal(label)!=0){
			fprintf(stderr,"%s:%d: ERROR: illegal label\n", context->fileName, context->lineNumber);
			return ILLEGAL_LABEL;
		}

		labelFlag = 1;
		p = skipWhiteSpaces(nextToken + 1);
		nextToken = readToken(p, " \t");

	}

	/*now p points to the first non-label token (word like command or .data) */
	/* now nextToken points to first character in first argument (first operand)*/
	/*now we expect one of the following: .data, .string, .entry, .extern or command */
	if (*p == '.') {
		/* this is an instruction line*/
		/* add label to symboltable if flag is 1 and not extern/entry */
		if (strncmp(p, ".entry",DOT_ENTRY) == 0) {
			if(pass==2){

			}
			/* we can ignore label defined in .extern line if firstpass */
			/* .entry lines arent processed in firstpass */
		} else if (strncmp(p, ".extern", DOT_EXTERN) == 0) {
			/* we can ignore label defined in .extern line */
			p+=DOT_EXTERN;
			if(p==skipWhiteSpaces(p)){
				fprintf(stderr,"%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
				return SYNTAX_ERROR;
			}
			nextToken = skipWhiteSpaces(nextToken);
			words = processDotExtern(context,".extern", nextToken,pass);
			if(words<0){
				/* processDotData failed and printed errors*/
				return words;
			}
			/*successful parse - one word for each operand and one word for .data*/
			/* update dataCount*/
			if(pass==1){
				context->dataCount+=words;
			}
			return 0;

		} else if (strncmp(p, ".data",DOT_DATA) == 0) {
			/* .data line */

			p+=DOT_DATA;
			if(p==skipWhiteSpaces(p)){
				fprintf(stderr,"%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
				return SYNTAX_ERROR;
			}
			words = processDotData(context,nextToken,pass);
			if(words<0){
				/* errors were printed in processDotData */
				return SYNTAX_ERROR;
			}
			if(pass==1){
				if(labelFlag==1){
					/*add newly defined symbol to symboltable*/
					add_symbol(&context->table, create_symbol(&context->table, label, context->dataCount, Regular,
							Data));	}
				/*successful parse - one word for each operand and one word for .data*/
				/* update data count*/
				context->dataCount+=(words);
			}
			return 0;

		} else if (strncmp(p, ".string",DOT_STRING) == 0) {
			/* .string line */

			p+=DOT_STRING;
			if(p==skipWhiteSpaces(p)){
				fprintf(stderr,"%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
				return SYNTAX_ERROR;
			}
			words = processDotString(context,nextToken,pass);
			if(words<0){
				/* errors were printed in processDotString */
				return SYNTAX_ERROR;
			}
			if(pass==1){
				if(labelFlag==1){
					/*add newly defined symbol to symboltable*/
					add_symbol(&context->table, create_symbol(&context->table, label, context->dataCount, Regular,
							Data));	}
				/*successful parse - one word for each operand and one word for .data*/
				/*update dataCount*/
				context->dataCount+=(words);
			}
			return 0;

		} else {
			fprintf(stderr,"%s:%d: ERROR: no such directive exists\n", context->fileName, context->lineNumber);
			return SYNTAX_ERROR;
		}
	} else {
		/*we expect a command here. add symbol if defined in this line*/
		if (labelFlag == 1 && pass == FIRST_PASS)
			add_symbol(&context->table, create_symbol(&context->table, label, context->instructionCount + 100, Regular,
					Code));

		if (nextToken - p == 0 || nextToken - p >= MAX_CMD) {
			fprintf(stderr,"%s:%d: ERROR: command expected\n", context->fileName, context->lineNumber);
			return COMMAND_EXPECTED;
		}
		strncpyNull(cmd, p, nextToken - p);
		cmdInfo = findCommand(cmd);
		if (cmdInfo == NULL) {
			fprintf(stderr,"%s:%d: ERROR: syntax error: unknown command and whitespace after label are illegal\n", context->fileName, context->lineNumber);
			return COMMAND_DOESNT_EXIST;
		}
		nextToken = skipWhiteSpaces(nextToken);
		result = cmdInfo->processCommand(context, cmdInfo, cmd, nextToken, pass);

		if (result < 0) {
			return result;
		}

        context->instructionCount += result;

	}

	return 0;
}


struct assemblerContext* createAssemblerContext(char* fileName) {
    struct assemblerContext* context = (struct assemblerContext*) calloc(1, sizeof (struct assemblerContext));
    /* calloc already initialized to zeroes. we take care of the rest */
    context->fileName = strdup(fileName);
    return context;
}

void deallocateAssemblerContext(struct assemblerContext* p) {
    free(p->fileName);
    dealloc_symbol_table(&p->table);
}

int assembler(char* fileName) {
    int result;
    struct assemblerContext* context = createAssemblerContext(fileName);
    result = firstPass(context);
    deallocateAssemblerContext(context);
    return result;
}

