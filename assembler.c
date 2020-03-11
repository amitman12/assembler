#include <stdio.h>
#include "assembler.h"
#include "utils.h"
#include "constants.h"

struct commandInfo* findCommand(char* cmd) {
    struct commandInfo* c;
    for (c = commands; c->command != NULL; ++c) {
        if (strcmp(c->command, cmd) == 0) {
            return c;
        }
    }
    return NULL;
}

char* readCommandOperand(char* p, struct operand* op) {
	/* puts addressing in op */
	/* p points to first char in operand (isn't whitespace)*/
	/* returns pointer to first char in the next operand */
    char* nextToken;
    op->addressingType = 0;

    if (*p == '*') {
        nextToken = readRegister(p,&(op->info.registerId));
        if (nextToken == NULL) {
            printf("expecting a valid register name");
            return NULL;
        }
        op->addressingType = Indirect_Register_Addressing;
        op->operandType = Register;

    } else if (*p == '#') {
        nextToken = readNumber(p, &op->info.immediateValue);
        if (nextToken == NULL) {
            printf("expecting a valid register name");
            return NULL;
        }
        op->addressingType = Immediate_Addressing;
        op->operandType = Immediate;

    } else {
    	nextToken = readRegister(p,&(op->info.registerId));
    	if(nextToken!=NULL){
            op->addressingType = Direct_Register_Addressing;
            op->operandType = Register;
    	}
    	nextToken = readSymbol(p,(op->info.symbolName));
    	if(nextToken!=NULL){
            op->addressingType = Direct_Addressing;
            op->operandType = Label;

    	}
    	printf("expected valid operand");
    	return NULL;
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



int processGroup1Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
	/*commands of group 1 - mov,add,sub */
	/* args points to first char of first argument in line */
    char* p;
    char* nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(p, &op1);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != ',') {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    ++p;

    nextToken = readCommandOperand(p, &op2);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf ("command %s accepts exactly two operands", cmd);
        return -1;
    }
    /* both operands were parsed successfully*/
    if ((op1.addressingType == 2||op1.addressingType==3) && (op2.addressingType == 2||op2.addressingType==3)) {
    	/* we only need one extra word in machine code for both operands */
        return 2;
    }
    else if(op2.addressingType == 0){
    	printf("error, dest operand addressing cannot be immediate addressing");
    	return ADDRESSING_ERROR;
    }
    else{
    	/* we need one word in machine code per operand and one for the command */
    	return 3;
    }
    return SYNTAX_ERROR;
}

int processGroup2Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
	/* command of group 2 - cmp */
    char* p;
    char* nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(p, &op1);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != ',') {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    ++p;

    nextToken = readCommandOperand(p, &op2);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf ("command %s accepts exactly two operands", cmd);
        return -1;
    }
    /* both operands were parsed successfully*/
    if ((op1.addressingType == 2||op1.addressingType==3) && (op2.addressingType == 2||op2.addressingType==3)) {
    	/* we only need one extra word in machine code for both operands */
        return 2;
    }
    else{
    	/* all addressing methods are legal for this command */
    	/* we need one word in machine code per operand and one for the command */
    	return 3;
    }
    return SYNTAX_ERROR;
}


int processGroup3Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
	/*commands of group 3 - lea */
	/* args points to first char of first argument in line */
    char* p;
    char* nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(p, &op1);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != ',') {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    ++p;

    nextToken = readCommandOperand(p, &op2);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf ("command %s accepts exactly two operands", cmd);
        return -1;
    }
    /* both operands were parsed successfully*/
    if(op1.addressingType != 1){
    	printf("error, source addressing type has to be Direct Addressing");
    	return ADDRESSING_ERROR;
    }
    else{
    	/* we need one word in machine code per operand and one for the command */
    	return 3;
    }
    return SYNTAX_ERROR;
}

int processGroup4Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
	/* group 4 commands - clr, not, inc, dec, red */
	/* these commands only take 1 operand */
    char* p;
    char* nextToken;
    struct operand op;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(p, &op);
    if (nextToken == NULL) {
        printf("command %s requires one operand", cmd);
        return SYNTAX_ERROR;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf("command %s requires exactly one operand", cmd);
        return SYNTAX_ERROR;
    }

    /* operand was parsed successfully*/
    if(op.addressingType ==0){
    	printf("error, dest addressing type cannot be Immediate Addressing");
    	return ADDRESSING_ERROR;
    }
    else{
    	/* we need one word in machine code per operand and one for the command */
    	/* one operand , one word*/
    	return 2;
    }
    return SYNTAX_ERROR;
}

int processGroup5Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
	/* group 5 commands - jmp, jsr, bne*/
	/* these commands only take 1 operand */
    char* p;
    char* nextToken;
    struct operand op;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(p, &op);
    if (nextToken == NULL) {
        printf("command %s requires one operand", cmd);
        return SYNTAX_ERROR;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf("command %s requires exactly one operand", cmd);
        return SYNTAX_ERROR;
    }

    /* operand was parsed successfully*/
    if(op.addressingType ==0||op.addressingType==3){
    	printf("error, dest addressing type cannot be Immediate Addressing or Direct Register Addressing");
    	return ADDRESSING_ERROR;
    }
    else{
    	/* we need one word in machine code per operand and one for the command */
    	/* one operand , one word*/
    	return 2;
    }
    return SYNTAX_ERROR;
}

int processGroup6Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
	/* group 6 commands - prn*/
	/* this command only takes 1 operand */
    char* p;
    char* nextToken;
    struct operand op;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(p, &op);
    if (nextToken == NULL) {
        printf("command %s requires one operand", cmd);
        return SYNTAX_ERROR;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf("command %s requires exactly one operand", cmd);
        return SYNTAX_ERROR;
    }

    /* operand was parsed successfully*/
    /* every addressing method is legal for prn */
    /* we need one word in machine code per operand and one for the command */
    /* one operand , one word*/
    return 2;
}

int processGroup7Command(struct commandInfo* cmdInfo, char* cmd, char* args) {
    char* p;
    p = skipWhiteSpaces(args);
    if (*p != '\0') {
        printf ("command %s does not accept arguments", cmd);
        return SYNTAX_ERROR;
    }
    /* one word in machine code needed*/
    return 1;
}

struct commandInfo commands[] = {
    {"mov", processGroup1Command},
    {"add", processGroup1Command},
    {"sub", processGroup1Command},
    {"cmp", processGroup2Command},
    {"lea", processGroup3Command},
    {"clr", processGroup4Command},
    {"not", processGroup4Command},
    {"inc", processGroup4Command},
    {"dec", processGroup4Command},
    {"red", processGroup4Command},
    {"jmp", processGroup5Command},
    {"jsr", processGroup5Command},
    {"bne", processGroup5Command},
    {"prn", processGroup6Command},
    {"rts", processGroup7Command},
    {"stop", processGroup7Command},
    {NULL, NULL}
};

int processLine( int lineNumber, char* line, int* dataCount, int* instructionCount) {
	/* returns 0 on successful parse */
	char* end;
	char* string;
	int label_operand_count = 0;
	int operand_count = 0;
	int word_count = 0;
    char* p = line;
    char* nextToken;
    char label[MAX_LABEL];
    char cmd[MAX_CMD];
    int result;
    int labelFlag = 0;
    struct commandInfo* cmdInfo;

    p = skipWhiteSpaces(p);
    if (*p == ';'||*p=='\0') {
    	/* comment line*/
        return 0;
    }
    /* we assume that after a label name there cannot be spaces */
    nextToken = readToken(p, " \t:");
    if (*nextToken == ':') {
    	/* new label defined in this line*/
        if (nextToken - p == 0) {
            printf("expecting non empty label");
            return LABEL_EMPTY;
        }
        if (nextToken - p >= MAX_LABEL) {
            printf("label too long");
            return LABEL_TOO_LONG;
        }
        strncpy (label, p, nextToken - p);
        if(find_symbol(label)){
        	printf("label already exists");
        	return LABEL_ALREADY_EXISTS;
        }
        if(is_reserved_word(label)){
        	printf("label can't be a reserved word");
        	return RESERVED_WORD;
        }
        labelFlag=1;
        p = skipWhiteSpaces(nextToken+1);
        nextToken = readToken(p, " \t");
        nextToken = skipWhiteSpaces(nextToken);
    }

    /*now p points to the first non-label token (word like command or .data) */
    /* now nextToken points to first character in first argument (first operand)*/
    /*now we expect one of the following: .data, .string, .entry, .extern or command */
    if (*p == '.') {
        /* this is an instruction line*/
    	/* add label to symboltable if flag is 1 and not extern/entry */
        if (strcmp(p, ".entry ")==0||strcmp(p, ".entry\t")==0) {
        	/* we can ignore label defined in .extern line */
        	/* .entry lines arent processed in firstpass */
        }else if (strcmp(p, ".extern ")==0||strcmp(p, ".extern\t")==0) {
        	/* we can ignore label defined in .extern line */
        	while(*nextToken!='\0'){
        		nextToken = readSymbol(nextToken,string);
        		if(nextToken==NULL){
        			/* readSymbol failed */
        			printf("syntax error");
        			return SYNTAX_ERROR;
        			}
        		if(checkLabel(string)){
        			/*add label to symboltable*/
    				add_symbol(create_symbol(string, *dataCount, External, Unknown_Location));

        		}

        		if(*nextToken=='\0'){
        			if(label_operand_count==0){
        				printf(".extern line with no labels");
        				return SYNTAX_ERROR;
        			}
        			return label_operand_count;
        		}
        		}
        	}

        }else if (strcmp(p, ".data ")==0||strcmp(p, ".data\t")==0) {
        	/* .data line */
        	while(*nextToken!='\0'){
        		if(*nextToken=='+'||*nextToken=='-'){
        			nextToken++;
        		}

        		if(!isdigit(*nextToken)){
        			printf("invalid argument for .data");
        			return SYNTAX_ERROR;
        		}
        		while(isdigit(*nextToken)){
        			nextToken++;
        		}
        		operand_count++;
        		nextToken = skipWhiteSpaces(nextToken);
        		if(*nextToken=='\0'){
        			if(labelFlag==1){
        				/* add label defined in this line to symboltable */
        				add_symbol(create_symbol(label, *dataCount, Regular, Data));
        			}
        			/* we need 1 word per operand and 1 word for the .data instruction */
        			return operand_count+1;
        		}
        		else if(*nextToken!=','){
        			printf("expecting ,");
        			return SYNTAX_ERROR;
        		}
        	}
        	/* no operands for .data line */
        	return SYNTAX_ERROR;

        }else if (strcmp(p, ".string ")==0||strcmp(p, ".string\t")==0) {
    		if(*nextToken!='\"'){
    			printf("expecting\" ");
    			return SYNTAX_ERROR;
    		}
    		nextToken++;
        	if(!isprint(nextToken)){
        		printf("string must only contain printable ascii characters");
        		return SYNTAX_ERROR;
        	}
        	end = find_last_quote(nextToken);
        	while(isprint(nextToken)&&nextToken<end){
        		word_count++;
        		nextToken++;
        	}
        	if(nextToken!=end){
        		printf("label must only contain printable ascii characters");
        		return SYNTAX_ERROR;
        	}
        	if(word_count==0){
        		printf(".string line with empty string "" ");
        		return SYNTAX_ERROR;
        	}
        	nextToken++;
        	nextToken = skipWhiteSpaces(nextToken);

        	if(*nextToken!='\0'){
        		printf("invalid syntax for .string line");
        		return SYNTAX_ERROR;
        	}
			if(labelFlag==1){
				/* add label defined in this line to symboltable */
				add_symbol(create_symbol(label, *dataCount, Regular, Data));
			}
			/* we need 1 word per character in string ex "abcd" means 5 words, including one word for \0 */
			/* we also need 1 word for the .string line */
			return word_count+2;
        }
        else {
            printf("no such directive");
            return SYNTAX_ERROR;
        }
    } else {
        /*we expect a command here. add symbol if defined in this line*/
        if(labelFlag==1)
        	add_symbol(create_symbol(label,*instructionCount+100),Regular,Code);

        if (nextToken - p == 0 || nextToken - p >= MAX_CMD) {
            printf("expecting a command");
            return COMMAND_EXPECTED;
        }
        strncpy(cmd, p, nextToken - p);
        cmdInfo = findCommand(cmd);
        if (cmdInfo == NULL) {
            printf("no such command");
            return COMMAND_DOESNT_EXIST;
        }
        result = cmdInfo->processCommand(cmdInfo, cmd, nextToken);

        if (result < 0) {
            return result;
        }

        *instructionCount += result;
    }

    return 0;
}
int assembler(char* fileName) {
    return firstPass(fileName);
}
