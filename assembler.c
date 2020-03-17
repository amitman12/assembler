#include <stdio.h>
#include <ctype.h>
#include "symboltable.h"
#include "assembler.h"
#include "utils.h"
#include "constants.h"
#include "firstpass.h"

union OpCodeWord {
    unsigned short int value;
    struct {
        unsigned int external: 1;
        unsigned int relocatable: 1;
        unsigned int absolute: 1;
        unsigned int dstImmediate: 1;
        unsigned int dstDirect: 1;
        unsigned int dstDirectRegister: 1;
        unsigned int dstIndirectRegister: 1;
        unsigned int srcImmediate: 1;
        unsigned int srcDirect: 1;
        unsigned int srcDirectRegister: 1;
        unsigned int srcIndirectRegister: 1;
        unsigned int opcode: 4;
        unsigned int unused: 1;
    } info;
};

/* this word can be either at 2nd position or 3rd position
 * and includes register references
 * there can be either one register or two registers involved
 */
union RegisterWord {
    unsigned short int value;
    struct {
        unsigned int external: 1;
        unsigned int relocatable: 1;
        unsigned int absolute: 1;
        unsigned int dstRegister: 3;
        unsigned int srcRegister: 3;
        unsigned int unused: 6;
    } info;
};

union ReferenceWord {
    unsigned short int value;
    struct {
        unsigned int external: 1;
        unsigned int relocatable: 1;
        unsigned int absolute: 1;
        unsigned int address: 12;
    } info;
};

union ImmediateWord {
    unsigned short int value;
    struct {
        unsigned int external: 1;
        unsigned int relocatable: 1;
        unsigned int absolute: 1;
        unsigned int address: 12;
    } info;
};

void outDirective(struct assemblerContext *context) {
	/* this function creates fileName.ext, fileName.ent and copies from symboltable in context to those files */
    /* this function then copies data image to fileName.ob (.string and .data) in context->memory */
    /* this function is only called after second pass has finished */

}


int
codeCommand(struct assemblerContext *context, struct commandInfo *cmdInfo, struct operand *op1, struct operand *op2) {
	/* returns -1 if couldn't create/open output file*/
	/* returns 0 on successful write to output file */
    /* this function gets a command line and creates the binary words corresponding to that command*/
    /* uses         to put the converted machine code in output file */
    unsigned short int words[3]; /* create an array of max number of command words */
    FILE* output = fopenFileWithExt(context->fileName,"w","ob");
    union OpCodeWord *commandWord;
    union RegisterWord *registerWord1;
    union RegisterWord *registerWord2;
    union ImmediateWord *immWord1;
    union ImmediateWord *immWord2;
    union ReferenceWord *refWord1;
    union ReferenceWord *refWord2;
    int numWords;
    numWords = 1;
    if(output==NULL){
    	fprintf(stderr,"could not create output file.\n");
    	return -1;
    }
    memset(words, 0, sizeof(words));
    /* set words to zero */

    commandWord = (union OpCodeWord *) &words[0];
    commandWord->info.opcode = cmdInfo->commandNumber;
    commandWord->info.absolute = 1;

    if (op1 == NULL) {
        /*commands stop,rts*/
        /* these commands only have one word which is the command word */
    	if(strcmp(cmdInfo->command,"stop")==0){
    		commandWord->info.opcode = stop;
    	}
    	else{
    		commandWord->info.opcode = rts;
    	}
    } else {
        /* we have at least one operand */
        commandWord->info.srcImmediate = (op1->addressingType == Immediate_Addressing);
        commandWord->info.srcDirect = (op1->addressingType == Direct_Addressing);
        commandWord->info.srcIndirectRegister = (op1->addressingType == Indirect_Register_Addressing);
        commandWord->info.srcDirectRegister = (op1->addressingType == Direct_Register_Addressing);

        if (op2 == NULL) {
            /* we have exactly one operand */
            if (op1->addressingType == Direct_Register_Addressing ||
                op1->addressingType == Indirect_Register_Addressing) {
                /* first operand is a register  - this means the second one has to be a label or immediate*/
                /* in command group 1, second operand can't be immediate - so that means second operand is a label */
                registerWord1 = (union RegisterWord *) &words[1];
                /* initialize to zero */
                registerWord1->info.srcRegister = 0;
                registerWord1->info.dstRegister = op1->info.registerId;
                registerWord1->info.absolute = 1;
            } else if (op1->addressingType == Immediate_Addressing) {
                /* first operand is a number*/
                immWord1 = (union ImmediateWord *) &words[1];
                immWord1->info.address = op1->info.immediateValue;
                immWord1->info.absolute = 1;
            } else {
                /* first operand is a label */
                refWord1 = (union ReferenceWord *) &words[1];
                if (find_symbol(&context->table, op1->info.symbolName) == NULL) {
                    /*operand 1 is an external label*/
                    /* we leave bits 3 to 14 untouched */
                    refWord1->info.external = 1;
                } else {
                    /* operand 2 is a label defined in this file */
                    refWord1->info.address = find_symbol(&context->table, op1->info.symbolName)->address;
                    refWord1->info.absolute = 1;
                }
            }
        } else {
            commandWord->info.dstImmediate = (op2->addressingType == Immediate_Addressing);
            commandWord->info.dstDirect = (op2->addressingType == Direct_Addressing);
            commandWord->info.dstIndirectRegister = (op2->addressingType == Indirect_Register_Addressing);
            commandWord->info.dstDirectRegister = (op2->addressingType == Direct_Register_Addressing);

            if ((op1->addressingType == Direct_Register_Addressing ||
                 op1->addressingType == Indirect_Register_Addressing)
                && (op2->addressingType == Direct_Register_Addressing ||
                    op2->addressingType == Indirect_Register_Addressing)
                && (strcmp(cmdInfo->command, "mov") == 0 || strcmp(cmdInfo->command, "sub") == 0 ||
                    strcmp(cmdInfo->command, "add") == 0)) {
                /* we only need one extra word in machine code for both operands */
                ++numWords;
                registerWord1 = (union RegisterWord *) &words[1];
                registerWord1->info.absolute = 1;
                registerWord1->info.dstRegister = op2->info.registerId;
                registerWord1->info.srcRegister = op1->info.registerId;

                /* bits 3 - 5 get dest register */
                /* bits 6 - 8 get src register */
                /* bits 0 - 2 get A,R,E fields  - absolute is 1 and rest are 0*/
                /* bits 9 - 14 get zero */
            }
            numWords += 2;
            /* if we get here it means we need two more words */
            if (op1->addressingType == Direct_Register_Addressing ||
                op1->addressingType == Indirect_Register_Addressing) {
                /* first operand is a register  - this means the second one has to be a label or immediate*/
                /* in command group 1, second operand can't be immediate - so that means second operand is a label */
                registerWord1 = (union RegisterWord *) &words[1];
                /* initialize to zero */
                registerWord1->info.dstRegister = 0;
                registerWord1->info.srcRegister = op1->info.registerId;
                registerWord1->info.absolute = 1;
            } else if (op1->addressingType == Immediate_Addressing) {
                /* first operand is a number*/
                immWord1 = (union ImmediateWord *) &words[1];
                immWord1->info.address = op1->info.immediateValue;
                immWord1->info.absolute = 1;
            } else {
                /* first operand is a label */
                refWord1 = (union ReferenceWord *) &words[1];
                if (find_symbol(&context->table, op1->info.symbolName) == NULL) {
                    /*operand 1 is an external label*/
                    /* we leave bits 3 to 14 untouched */
                    refWord1->info.external = 1;
                }
                /* operand 2 is a label defined in this file */
                refWord1->info.address = find_symbol(&context->table, op1->info.symbolName)->address;
                refWord1->info.absolute = 1;
            }

            if (op2->addressingType == Direct_Register_Addressing ||
                op2->addressingType == Indirect_Register_Addressing) {
                /* second operand is a register */
                registerWord2 = (union RegisterWord *) &words[2];
                registerWord2->info.srcRegister = 0;
                registerWord2->info.dstRegister = op2->info.registerId;
                registerWord2->info.absolute = 1;
            } else if (op2->addressingType == Immediate_Addressing) {
                /* second operand is an immediate*/
                immWord2 = (union ImmediateWord *) &words[2];
                immWord2->info.address = op2->info.immediateValue;
                immWord2->info.absolute = 1;
            } else {
                /* second operand is a label */
                refWord2 = (union ReferenceWord *) &words[2];
                if (find_symbol(&context->table, op2->info.symbolName) == NULL) {
                    /*operand 2 is an external label*/
                    /* we leave bits 3 to 14 untouched */
                    refWord2->info.external = 1;
                }
                /* operand 2 is a label defined in this file */
                refWord2->info.address = find_symbol(&context->table, op2->info.symbolName)->address;
                refWord2->info.absolute = 1;
            }
        }
    }
    if(numWords==1){
    	fprintf(output, "%05\n",twosComplement(words[0]));
    	return 0;
    }
    else if(numWords==2){
    	fprintf(output, "%05\n",twosComplement(words[0]));
    	fprintf(output, "%05\n",twosComplement(words[1]));
    	return 0;
    }
    /* numWords==3 */
	fprintf(output, "%05\n",twosComplement(words[0]));
	fprintf(output, "%05\n",twosComplement(words[1]));
	fprintf(output, "%05\n",twosComplement(words[2]));
    return 0;
}


int codeNumber(struct assemblerContext *context, int number, int count) {
    /* reallocates space for .data argument and writes it in memory in correct base */
    /* returns 0 on success*/
    context->memory = (int *) realloc(context->memory, context->dataCount + count);
    /* each word in machine code is 15 bits which means we need 2 bytes per word. and a number is 1 word  */
    if (context->memory == NULL) {
        fprintf(stderr, "ERROR: could not allocate memory\n");
        free(context->memory);
        return -1;
    }
    context->memory[context->dataCount + count - 1] = number;
    /* we write into memory in location data count + count - 1 */
    return 0;
}

int codeString(struct assemblerContext *context, char *str) {
    char *p;
    int result;
    for (p = str; *p; ++p) {
        if ((result = codeNumber(context, p - str + 1, (int) *p)) != 0) {
            return result;
        }
    }
    /* copy the null termination */
    return codeNumber(context, p - str + 1, 0);
}


int processDotEntry(struct assemblerContext *context, char *directive, char *args) {
    /* args points to first argument */
    /* returns errors or number of machine code words needed*/
    /* adds extern labels to symboltable */
	/* we only call this function in second pass */
	/* modifies symbols to have .entry value */
    char *label = (char *) calloc(MAX_LABEL, sizeof(char));
    int count = 0;
    struct symbol* symbol;

    while (*args != '\0') {
        args = readSymbol(args, label);
        if (args == NULL) {
            fprintf(stderr, "%s:%d: ERROR: invalid argument for .extern \n", context->fileName, context->lineNumber);
            free(label);
            return ILLEGAL_LABEL;
        }
        if (is_reserved_word(label) == 1) {
            fprintf(stderr, "%s:%d: ERROR: argument for .extern can't be a reserved word \n", context->fileName,
                    context->lineNumber);
            free(label);
            return ILLEGAL_LABEL;
        }
        if (is_legal(label) == -1) {
            fprintf(stderr, "%s:%d: ERROR: illegal label argument for .extern \n", context->fileName,
                    context->lineNumber);
            free(label);
            return ILLEGAL_LABEL;
        }
        symbol = find_symbol(&context->table,label);
        if(symbol==NULL){
        	/* .entry labels have to be defined in this file. */
        	/* if we don't find the symbol it means it isn't in this file because this is second pass */
            fprintf(stderr, "%s:%d: ERROR: .entry symbol isnt defined. \n",context->fileName,context->lineNumber);
            free(label);
            return LABEL_ALREADY_EXISTS;
        }
        modify_symbol(&context->table,symbol->label,symbol->address,Entry,symbol->location);
        count++;
        if (*args == ',') {
            ++args;
            args = skipWhiteSpaces(args);
        }
    }
    if (count == 0) {
        fprintf(stderr, "%s:%d: ERROR: no arguments for .extern line \n", context->fileName, context->lineNumber);
        free(label);
        return LABEL_EMPTY;
    }
    /* return number of extern labels in this line */
    free(label);
    return count;}


int processDotExtern(struct assemblerContext *context, char *directive, char *args) {
    /* args points to first argument */
    /* returns errors or number of machine code words needed*/
    /* adds extern labels to symboltable */
    char *label = (char *) calloc(MAX_LABEL, sizeof(char));
    int count = 0;

    while (*args != '\0') {
        args = readSymbol(args, label);
        if (args == NULL) {
            fprintf(stderr, "%s:%d: ERROR: invalid argument for .extern \n", context->fileName, context->lineNumber);
            free(label);
            return ILLEGAL_LABEL;
        }
        if (is_reserved_word(label) == 1) {
            fprintf(stderr, "%s:%d: ERROR: argument for .extern can't be a reserved word \n", context->fileName,
                    context->lineNumber);
            free(label);
            return ILLEGAL_LABEL;
        }
        if (is_legal(label) == -1) {
            fprintf(stderr, "%s:%d: ERROR: illegal label argument for .extern \n", context->fileName,
                    context->lineNumber);
            free(label);
            return ILLEGAL_LABEL;
        }
        /*if(find_symbol(&context->table,label)!=NULL){
            fprintf(stderr, "%s:%d: ERROR: illegal label argument for .extern \n",context->fileName,context->lineNumber);
            free(label);
            return LABEL_ALREADY_EXISTS;
        }*/
        /* we don't check if a label given as argument in .extern line already exists in symboltable*/
        if (context->pass == FIRST_PASS) {
            add_symbol(&context->table, create_symbol(&context->table, label, UNKNOWN_ADDRESS, External,
                                                      Unknown_Location));
        }
        count++;
        if (*args == ',') {
            ++args;
            args = skipWhiteSpaces(args);
        }
    }
    if (count == 0) {
        fprintf(stderr, "%s:%d: ERROR: no arguments for .extern line \n", context->fileName, context->lineNumber);
        free(label);
        return LABEL_EMPTY;
    }
    /* return number of extern labels in this line */
    free(label);
    return count;
}

int processDotData(struct assemblerContext *context, char *args) {
    /* args points to first argument */
    /* returns errors or number of words needed in machine code for this .string line  on success */
    int count = 0;
    int current_immediate = 0;
    int *temp = &current_immediate;
    int err = 0;
    int *err_type = &err;

    if (args == '\0') {
        fprintf(stderr, "%s:%d: ERROR: .data directive can't have an empty immediate \n", context->fileName,
                context->lineNumber);
        return -1;
    }
    while (*args != '\0') {
        args = readNumber(args, temp, err_type);
        if (*err_type == EMPTY_IMMEDIATE) {
            fprintf(stderr, "%s:%d: ERROR: .data directive wrong operand type or empty immediate \n", context->fileName,
                    context->lineNumber);
            return -1;
        }
        if (*err_type == SYNTAX_ERROR) {
            fprintf(stderr, "%s:%d: ERROR: .data directive syntax error\n", context->fileName, context->lineNumber);
            return -1;
        }
        count++;
        /* immediate is in *temp */
        if (context->pass == FIRST_PASS) {
            codeNumber(context, *temp, count);
        }
        if (*args == ',') {
            args++;
            args = skipWhiteSpaces(args);
            /* now points to next number*/
        }
    }
    /* we need 1 word per immediate plus one for the .data directive*/
    return count;
}

int processDotString(struct assemblerContext *context, char *args) {
    /* args points to first argument */
    /* returns errors or number of words needed in machine code for this .string line  on success */
    char str[MAX_CMD];
    int len;
    readString(args, str, &len);
    if (len == EMPTY_STRING) {
        fprintf(stderr, "%s:%d: ERROR: .string directive can't have an empty string\n", context->fileName,
                context->lineNumber);
        return -1;

    }
    if (len == SYNTAX_ERROR) {
        fprintf(stderr, "%s:%d: ERROR: .string directive syntax error\n", context->fileName, context->lineNumber);
        return -1;

    }
    /* string is in str*/
    if (context->pass == FIRST_PASS) {
        codeString(context, str);
    }
    /* we need one word of machine code per char in the string(including \0) plus one for the command */
    return len + 1;
}


struct commandInfo *findCommand(char *cmd) {
    /*return pointer to commandInfo if found or NULL if not*/
    struct commandInfo *c;
    for (c = commands; c->command != NULL; ++c) {
        if (strcmp(c->command, cmd) == 0) {
            return c;
        }
    }
    return NULL;
}

char *readCommandOperand(struct assemblerContext *context, char *p, struct operand *op, int isCommand) {
    /* puts addressing in op */
    /* p points to first char in operand (isn't whitespace)*/
    /* returns pointer to first char in the next operand */
    /* is command is 1 if line is a command line and 0 on directive line */
    int *err_type;
    char *nextToken;
    err_type = (int *) malloc(sizeof(int));
    op->addressingType = 0;

    if (*p == '*' && isCommand == 1) {
        p++;
        nextToken = readRegister(p, &(op->info.registerId));
        if (nextToken == NULL) {
            free(err_type);
            if (op->info.registerId == INVALID_REGISTER) {
                fprintf(stderr, "%s:%d: ERROR: expecting a valid register name\n", context->fileName,
                        context->lineNumber);
            }
            return NULL;
        }
        op->addressingType = Indirect_Register_Addressing;
        op->operandType = Register;

    } else if (*p == '#' && isCommand == 1) {
        p++;
        nextToken = readNumber(p, &op->info.immediateValue, err_type);
        if (*err_type == SYNTAX_ERROR) {
            free(err_type);
            fprintf(stderr, "%s:%d: ERROR: expecting a valid immediate\n", context->fileName, context->lineNumber);
            return NULL;
        } else if (*err_type == EMPTY_IMMEDIATE) {
            free(err_type);
            fprintf(stderr, "%s:%d: ERROR: immediate can't be empty\n", context->fileName, context->lineNumber);
            return NULL;
        }
        free(err_type);
        op->addressingType = Immediate_Addressing;
        op->operandType = Immediate;

    } else if (isCommand == 1) {
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

            } else if (isCommand == 0) {
                nextToken = readNumber(p, &(op->info.immediateValue), err_type);
                if (nextToken != NULL) {
                    op->addressingType = Direct_Addressing;
                    op->operandType = Immediate;
                    free(err_type);
                }
            }
        }


        if (nextToken == NULL) {
            free(err_type);
            fprintf(stderr, "%s:%d: ERROR: expected valid operand\n", context->fileName, context->lineNumber);
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

int
processGroup1Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    /*commands of group 1 - mov,add,sub */
    /* args points to first char of first argument in line */
    char *p;
    char *nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(context, p, &op1, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
                cmdInfo->command);
        return -1;
    }
    nextToken = skipWhiteSpaces(nextToken);
    if (*nextToken != ',') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    nextToken++;
    nextToken = skipWhiteSpaces(nextToken);

    nextToken = readCommandOperand(context, nextToken, &op2, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    /* both operands were parsed successfully*/
    if (context->pass == FIRST_PASS) {
        if ((op1.addressingType == 2 || op1.addressingType == 3)
            && (op2.addressingType == 2 || op2.addressingType == 3)) {
            /* we only need one extra word in machine code for both operands */
            return 2;
        } else if (op2.addressingType == 0) {
            fprintf(stderr, "%s:%d: ERROR: dest operand addressing of command %s cannot be immediate addressing\n",
                    context->fileName, context->lineNumber, cmdInfo->command);
            return ADDRESSING_ERROR;
        } else {
            /* we need one word in machine code per operand and one for the command */
            return 3;
        }
    }
    codeCommand(context, cmdInfo, &op1, &op2);
    /* successful parse and second parse - we code here */
    /* command is in cmd. operand 1 is in op1 and operand 2 is in op2 */
    return 1;


}

int
processGroup2Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    /* command of group 2 - cmp */
    char *p;
    char *nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(context, p, &op1, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    nextToken = skipWhiteSpaces(nextToken);
    if (*nextToken != ',') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    ++nextToken;
    nextToken = skipWhiteSpaces(nextToken);

    nextToken = readCommandOperand(context, nextToken, &op2, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    nextToken = skipWhiteSpaces(nextToken);
    if (*nextToken != '\0') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires exactly two operands\n", context->fileName,
                context->lineNumber, cmdInfo->command);
        return -1;
    }
    /* both operands were parsed successfully*/
    if (context->pass == FIRST_PASS) {
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
    codeCommand(context, cmdInfo, &op1, &op2);
    /* successful parse and second parse - we code here */
    /* command is in cmdInfo->command. operand 1 is in op1 and operand 2 is in op2 */
    return 1;

}

int
processGroup3Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    /*commands of group 3 - lea */
    /* args points to first char of first argument in line */
    char *p;
    char *nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(context, p, &op1, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    nextToken = skipWhiteSpaces(nextToken);
    if (*nextToken != ',') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    ++nextToken;
    nextToken = skipWhiteSpaces(nextToken);


    nextToken = readCommandOperand(context, nextToken, &op2, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    nextToken = skipWhiteSpaces(nextToken);
    if (*nextToken != '\0') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires two operands\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return -1;
    }
    /* both operands were parsed successfully*/
    if (context->pass == FIRST_PASS) {
        if (op1.addressingType != 1) {
            fprintf(stderr, "%s:%d: ERROR: source addressing type of command %s has to be Direct Addressing\n",
                    context->fileName, context->lineNumber, cmdInfo->command);
            return ADDRESSING_ERROR;
        } else {
            if (op2.addressingType == 0) {
                fprintf(stderr, "%s:%d: ERROR: target addressing type of command %s cannot be Immediate Addressing\n",
                        context->fileName, context->lineNumber, cmdInfo->command);
                return ADDRESSING_ERROR;
            }
            /* we need one word in machine code per operand and one for the command */
            return 3;
        }
    }
    /* if we get here we're in 2nd pass */
    codeCommand(context, cmdInfo, &op1, &op2);
    /* successful parse and second parse - we code here */
    /* command is in cmdInfo->command. operand 1 is in op1 and operand 2 is in op2 */
    return 1;
}

int
processGroup4Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    /* group 4 commands - clr, not, inc, dec, red */
    /* these commands only take 1 operand */
    char *p;
    char *nextToken;
    struct operand op;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(context, p, &op, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s requires exactly one operand\n", context->fileName,
                context->lineNumber, cmdInfo->command);
        return SYNTAX_ERROR;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        fprintf(stderr, "%s:%d: ERROR: command %s requires exactly one operand\n", context->fileName,
                context->lineNumber, cmdInfo->command);
        return SYNTAX_ERROR;
    }

    /* operand was parsed successfully*/
    if (context->pass == FIRST_PASS) {
        if (op.addressingType == 0) {
            fprintf(stderr, "%s:%d: ERROR: dest addressing type of command %s cannot be Immediate Addressing\n",
                    context->fileName, context->lineNumber, cmdInfo->command);
            return ADDRESSING_ERROR;
        } else {
            /* we need one word in machine code per operand and one for the command */
            /* one operand , one word*/
            return 2;
        }
    }
    /* if we get here we're in 2nd pass */
    codeCommand(context, cmdInfo, &op, NULL);
    /* successful parse and second parse - we code here */
    /* command is in cmdInfo->command. operand 1 is in op and operand 2 is NULL */
    return 1;
}

int
processGroup5Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    /* group 5 commands - jmp, jsr, bne*/
    /* these commands only take 1 operand */
    char *p;
    char *nextToken;
    struct operand op;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(context, p, &op, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR:command %s requires exactly one operand\n", context->fileName,
                context->lineNumber, cmdInfo->command);
        return SYNTAX_ERROR;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        fprintf(stderr, "%s:%d: ERROR:command %s requires exactly one operand\n", context->fileName,
                context->lineNumber, cmdInfo->command);
        return SYNTAX_ERROR;
    }

    /* operand was parsed successfully*/
    if (context->pass == FIRST_PASS) {
        if (op.addressingType == 0 || op.addressingType == 3) {
            fprintf(stderr,
                    "%s:%d: ERROR: dest addressing type cannot be Immediate Addressing or Direct Register Addressing\n",
                    context->fileName, context->lineNumber);
            return ADDRESSING_ERROR;
        } else {
            /* we need one word in machine code per operand and one for the command */
            /* one operand , one word*/
            return 2;
        }
    }
    /* if we get here we're in 2nd pass */
    codeCommand(context, cmdInfo, &op, NULL);
    /* successful parse and second parse - we code here */
    /* command is in cmdInfo->command. operand 1 is in op and op2 is NULL*/
    return 1;
}

int
processGroup6Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    /* group 6 commands - prn*/
    /* this command only takes 1 operand */
    char *p;
    char *nextToken;
    struct operand op;

    p = skipWhiteSpaces(args);
    /* p points to the first non whitespace character in args */
    nextToken = readCommandOperand(context, p, &op, 1);
    if (nextToken == NULL) {
        fprintf(stderr, "%s:%d: ERROR: command %s takes exactly 1 operand\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return SYNTAX_ERROR;
    }
    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        fprintf(stderr, "%s:%d: ERROR: command %s takes exactly 1 operand\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return SYNTAX_ERROR;
    }

    /* operand was parsed successfully*/
    /* every addressing method is legal for prn */
    /* we need one word in machine code per operand and one for the command */
    /* one operand , one word*/
    if (context->pass == FIRST_PASS) {
        return 2;
    }
    /* if we get here we're in 2nd pass */
    codeCommand(context, cmdInfo, &op, NULL);
    /* successful parse and second parse - we code here */
    /* command is in cmdInfo->command. operand 1 is in op1*/
    return 1;
}

int
processGroup7Command(struct assemblerContext *context, struct commandInfo *cmdInfo, char *args) {
    char *p;
    p = skipWhiteSpaces(args);
    if (*p != '\0') {
        fprintf(stderr, "%s:%d: ERROR: command %s does not accept arguments\n", context->fileName, context->lineNumber,
        		cmdInfo->command);
        return SYNTAX_ERROR;
    }
    /* one word in machine code needed*/
    if (context->pass == FIRST_PASS) {
        return 1;
    }
    /* if we get here we're in 2nd pass */
    codeCommand(context, cmdInfo, NULL, NULL);
    /* successful parse and second parse - we code here */    /* if we get here we're in 2nd pass */
    /* command is in cmdInfo->command. operands are both NULL*/
    return 1;
    /* command is in cmdInfo->command. operand 1 is in op1*/
}

struct commandInfo commands[] = { /**/
        {"mov",  mov,  processGroup1Command}, /**/
        {"add",  add,  processGroup1Command}, /**/
        {"sub",  sub,  processGroup1Command}, /**/
        {"cmp",  cmp,  processGroup2Command}, /**/
        {"lea",  lea,  processGroup3Command}, /**/
        {"clr",  clr,  processGroup4Command}, /**/
        {"not",  not,  processGroup4Command}, /**/
        {"inc",  inc,  processGroup4Command}, /**/
        {"dec",  dec,  processGroup4Command}, /**/
        {"red",  red,  processGroup4Command}, /**/
        {"jmp",  jmp,  processGroup5Command}, /**/
        {"jsr",  jsr,  processGroup5Command}, /**/
        {"bne",  bne,  processGroup5Command}, /**/
        {"prn",  prn,  processGroup6Command}, /**/
        {"rts",  rts,  processGroup7Command}, /**/
        {"stop", stop, processGroup7Command}, /**/
        {NULL, NULL} /**/
};

int processLine(struct assemblerContext *context, char *line) {
    /* returns number of machine code words needed for this line on successful parse */
    /* returns negatives on errors */
    /* pass = 1 on firstpass. pass = 2 on secondpass */
    int words;
    char *p = line;
    char *nextToken;
    char label[MAX_LABEL];
    char cmd[MAX_CMD];
    int result;
    int labelFlag = 0;
    struct commandInfo *cmdInfo;
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
            fprintf(stderr, "%s:%d: ERROR: expecting non empty label\n", context->fileName, context->lineNumber);
            return LABEL_EMPTY;
        }
        if (nextToken - p > MAX_LABEL) {
            fprintf(stderr, "%s:%d: ERROR: label too long\n", context->fileName, context->lineNumber);
            return LABEL_TOO_LONG;
        }
        strncpyNull(label, p, nextToken - p);
        if (find_symbol(&context->table, label)) {
            fprintf(stderr, "%s:%d: ERROR: label already exists\n", context->fileName, context->lineNumber);
            return LABEL_ALREADY_EXISTS;
        }
        if (is_reserved_word(label)) {
            fprintf(stderr, "%s:%d: ERROR: label can't be a reserved word\n", context->fileName, context->lineNumber);
            return RESERVED_WORD;
        }
        if (is_legal(label) != 0) {
            fprintf(stderr, "%s:%d: ERROR: illegal label\n", context->fileName, context->lineNumber);
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
        if (strncmp(p, ".entry", DOT_ENTRY) == 0) {
            if (context->pass == SECOND_PASS) {
                p += DOT_ENTRY;
                if (p == skipWhiteSpaces(p)) {
                    fprintf(stderr, "%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
                    return SYNTAX_ERROR;
                }
                nextToken = skipWhiteSpaces(nextToken);
                words = processDotEntry(context, ".extern", nextToken);
                if (words < 0) {
                    /* processDotData failed and printed errors*/
                    return words;
                }
                /*successful parse - one word for each operand and one word for .data*/
                /* update dataCount*/
                if (context->pass == FIRST_PASS) {
                    context->dataCount += words;
                }
                return 0;
            }
            /* we can ignore label defined in .extern line if firstpass */
            /* .entry lines arent processed in firstpass */
        } else if (strncmp(p, ".extern", DOT_EXTERN) == 0) {
            /* we can ignore label defined in .extern line */
            p += DOT_EXTERN;
            if (p == skipWhiteSpaces(p)) {
                fprintf(stderr, "%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
                return SYNTAX_ERROR;
            }
            nextToken = skipWhiteSpaces(nextToken);
            words = processDotExtern(context, ".extern", nextToken);
            if (words < 0) {
                /* processDotData failed and printed errors*/
                return words;
            }
            /*successful parse - one word for each operand and one word for .data*/
            /* we don't update dataCount on .extern*/
            return 0;

        } else if (strncmp(p, ".data", DOT_DATA) == 0) {
            /* .data line */

            p += DOT_DATA;
            if (p == skipWhiteSpaces(p)) {
                fprintf(stderr, "%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
                return SYNTAX_ERROR;
            }
            words = processDotData(context, nextToken);
            if (words < 0) {
                /* errors were printed in processDotData */
                return SYNTAX_ERROR;
            }
            if (context->pass == FIRST_PASS) {
                if (labelFlag == 1) {
                    /*add newly defined symbol to symboltable*/
                    add_symbol(&context->table, create_symbol(&context->table, label, context->dataCount, Regular,
                                                              Data));
                }
                /*successful parse - one word for each operand and one word for .data*/
                /* update data count*/
                context->dataCount += words;
            }
            return 0;

        } else if (strncmp(p, ".string", DOT_STRING) == 0) {
            /* .string line */

            p += DOT_STRING;
            if (p == skipWhiteSpaces(p)) {
                fprintf(stderr, "%s:%d: ERROR: no such directive\n", context->fileName, context->lineNumber);
                return SYNTAX_ERROR;
            }
            words = processDotString(context, nextToken);
            if (words < 0) {
                /* errors were printed in processDotString */
                return SYNTAX_ERROR;
            }
            if (context->pass == FIRST_PASS) {
                if (labelFlag == 1) {
                    /*add newly defined symbol to symboltable*/
                    add_symbol(&context->table, create_symbol(&context->table, label, context->dataCount, Regular,
                                                              Data));
                }
                /*successful parse - one word for each operand and one word for .data*/
                /*update dataCount*/
                context->dataCount += words;
            }
            return 0;

        } else {
            fprintf(stderr, "%s:%d: ERROR: no such directive exists\n", context->fileName, context->lineNumber);
            return SYNTAX_ERROR;
        }
    } else {
        /*we expect a command here. add symbol if defined in this line*/
        if (labelFlag == 1 && context->pass == FIRST_PASS)
            add_symbol(&context->table, create_symbol(&context->table, label, context->instructionCount + 100, Regular,
                                                      Code));

        if (nextToken - p == 0 || nextToken - p >= MAX_CMD) {
            fprintf(stderr, "%s:%d: ERROR: command expected\n", context->fileName, context->lineNumber);
            return COMMAND_EXPECTED;
        }
        strncpyNull(cmd, p, nextToken - p);
        cmdInfo = findCommand(cmd);
        if (cmdInfo == NULL) {
            fprintf(stderr, "%s:%d: ERROR: syntax error: unknown command and whitespace after label are illegal\n",
                    context->fileName, context->lineNumber);
            return COMMAND_DOESNT_EXIST;
        }
        nextToken = skipWhiteSpaces(nextToken);
        result = cmdInfo->processCommand(context, cmdInfo, cmd, nextToken);

        if (result < 0) {
            return result;
        }

        context->instructionCount += result;

    }

    return 0;
}


struct assemblerContext *createAssemblerContext(char *fileName) {
    struct assemblerContext *context = (struct assemblerContext *) calloc(1, sizeof(struct assemblerContext));
    /* calloc already initialized to zeroes. we take care of the rest */
    context->fileName = strdup(fileName);
    return context;
}

void deallocateAssemblerContext(struct assemblerContext *p) {
    if (p->objFile) {
        fclose(p->objFile);
    }
    free(p->fileName);
    dealloc_symbol_table(&p->table);
}

int assembler(char *fileName) {
    int result;
    struct assemblerContext *context = createAssemblerContext(fileName);
    result = firstPass(context);
    deallocateAssemblerContext(context);
    return result;
}

