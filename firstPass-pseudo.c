// h file


struct commandInfo {
    char* command;
    int (*processCommand(struct commandInfo*, char* cmd, char* args));
};

extern struct commandInfo* commands[];

// ----------------------------
// c file



char* skipWhiteSpaces(char* p) {
    while (*p == ' ' || *p == '\t') {
        ++p;
    }
    return p;
}

int isDelimiter(char ch, char* delimiters) {
    char *p;
    for (p = delimiters; *p; ++p) {
        if (*p == ch) {
            return 1;
        }
    }
    return 0;
}

char* readNumber(char*p, int* number) {

}

char* readSymbol(char*p) {
    if (!isalpha(*p)) {
        return NULL;
    }
    ++p;
    for(; isalnum(*p); ++p) {
    }
    if (*p == '\0' || isDelimiter(*p, " \t,")) {
        return p;
    }
    return NULL;
}

char* readToken(char*p, char* delimiters) {
    for (; *p; ++p) {
        if (isDelimiter(*p, delimiters)) {
            return p;
        }
    }
    return p;
}

//[*]?[ \t]r0..r7
//#[+-]?[0-9]+
//[a-z][a-z0-9]*
struct operand {
    int operandType; //0-register, 1-immediate, 2-symbol
    int addressingType; //default:0, *-reg: 1, #-2, 3
    union {
        int registerId;
        int immediateValue;
        char symbolName[100];
    };
};

char* readCommandOperand(char* p, struct operand* op) {
    char* nextToken;
    op->addressingType = 0;

    if (*p == '*') {
        nextToken = readSymbol(p);
        if (nextToken == NULL) {
            printf("expecting a register name");
            return NULL;
        }
        if (*p != 'r') {
            printf("expecting a register name");
            return NULL;
        }
        // check register number
        // ensure len is exactly 2;
        op->addressingType = 1;
    } else if (*p == '#') {
        nextToken = readNumber(p, &op->immediateValue);
        if (nextToken == NULL) {
            print...
            return NULL;
        }
        op->addressingType = 2;
    } else {
        // we expect a register or a symbol name
    }
    return nextToken;
}


/*

mov, add, sub, 
cmp
lea
clr, not, inc, dec, red
jmp, bne, jsr
prn
rts, stop

*/

int processAssignCommand(struct commandInfo* cmdInfo, char* cmd, char* args) {

    char* p;
    char* nextToken;
    struct operand op1;
    struct operand op2;

    p = skipWhiteSpaces(args);
    nextToken = readOperand(p, &op1);
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

    nextToken = readOperand(p, &op2);
    if (nextToken == NULL) {
        printf("command %s requires two operands", cmd);
        return -1;
    }

    if (op1.addressingType == 1 && op2.addressingType == 1) {
        printf("not allowed");
        return -1;
    }

    p = skipWhiteSpaces(nextToken);
    if (*p != '\0') {
        printf ("command %s accepts only two operands", cmd);
        return -1;
    }

    return 88;
}

int processCmpCommand(struct commandInfo* cmdInfo, char* cmd, char* args) {
    return -1;
}

int processLeaCommand(struct commandInfo* cmdInfo, char* cmd, char* args) {
    return -1;
}

int processACommand(struct commandInfo* cmdInfo, char* cmd, char* args) {
    return -1;
}

int processBranchCommand(struct commandInfo* cmdInfo, char* cmd, char* args) {
    return -1;
}

int processPrnCommand(struct commandInfo* cmdInfo, char* cmd, char* args) {
    return -1;
}

int processNonArgsCommand(struct commandInfo* cmdInfo, char* cmd, char* args) {
    char* p;
    p = skipWhiteSpaces(args);
    if (*p != '\0') {
        printf ("command %s does not accept args", cmd);
        return -1;
    }

    return 1;
}

struct commandInfo* commands[] = {
    {"mov", processAssignCommand},
    {"add", processAssignCommand},
    {"sub", processAssignCommand},
    {"cmp", processCmpCommand},
    {"lea", processLeaCommand},
    {"clr", processACommand},
    {"not", processACommand},
    {"inc", processACommand},
    {"dec", processACommand},
    {"red", processACommand},
    {"jmp", processBranchCommand},
    {"jsr", processBranchCommand},
    {"bne", processBranchCommand},
    {"prn", processPrnCommand},
    {"rts", processNonArgsCommand},
    {"stop", processNonArgsCommand},
    {NULL, NULL}
};

struct commandInfo* findCommand(char* cmd) {
    struct commandInfo* c;
    for (c = commands[0]; c->command != NULL; ++c) {
        if (strcmp(c->command, cmd) == 0) {
            return c;
        }
    }
    return NULL;
}

int processLine(int lineNumber, char* line, int* dataCount, int* instructionCount) {
    char* p = line;
    char* nextToken;
    char label[MAX_LABEL];
    char cmd[MAX_CMD];
    int result;
    struct commandInfo* cmdInfo;

    p = skipWhiteSpaces(p);
    if (*p == ';') {
        continue;
    }
    // we assume that after a label name there cannot be spaces
    nextToken = readToken(p, " \t:");
    if (*nextToken == ':') { // we found a label
        if (nextToken - p == 0) {
            printf("expecting non empty label");
            return -1;
        }
        if (nextToken - p >= MAX_LABEL) {
            printf("label too long");
            return -1;
        }
        strncpy (label, p, nextToken - p);
        p = skipWhiteSpaces(nextToken + 1);
        nextToken = readToken(p, " \t");
    }
    
    // now p points to the first non-label token
    // now we expect one of the folllowing: .data, .string, .entry, .extern or command
    if (*p == '.') {
        // this is an instruction
        if (strcmp(p, ".entry")) {

        } else if (strcmp(p, ".extern")) {

        } else {
            printf("no such directive");
            return -1;
        }
    } else {
        // we expect a command here
        if (nextToken - p == 0 || nextToken - p >= MAX_CMD_LEN) {
            printf("expecting a command");
            return -1;
        }
        strncpy(cmd, p, nextToken - p);

        cmdInfo = findCommand(cmd);
        if (cmdInfo == NULL) {
            printf("no such command");
            return -1;
        }
        result = cmdInfo->processCommand(cmd, nextToken);

        if (result < 0) {
            return result;
        }
        *instructionCount += result;
    }

    return 0;
}

int firstPass() {
    int dataCount = 0;
    int instructionCount = 0;
    int errorCount = 0;
    int lineNumber = 0;
    int result;
    FILE* f = fopen("test");
    if (f == NULL) {
        printf("err");
        return -1;
    }

    char line[150];
    while (fgets(line) != NULL) {
        if ((result = processLine(++lineNumber, line, &dataCount, &instructionCount)) < 0) {
            ++errorCount;
        }
    }

    if (errorCount == 0) {
        update_data_symbol(*instructionCount);
    }
    return errorCount;
}