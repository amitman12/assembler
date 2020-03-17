/*
 * assembler.h
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "symboltable.h"

struct assemblerContext {
	int pass;
    char* fileName;
    struct symboltable table;
    int lineNumber;
    int errorCount;
    int instructionCount;
    int dataCount;
    int* memory;
    FILE* objFile;
};

struct commandInfo {
    char* command;
    int commandNumber;
    int (*processCommand)(struct assemblerContext* context, struct commandInfo*, char* cmd, char* args);
};

extern struct commandInfo commands[];


struct operand {
    int operandType;
    int addressingType;
    union{
        int registerId;
        int immediateValue;
        char symbolName[100];
    } info;
};



struct assemblerContext* createAssemblerContext(char* fileName);
void deallocateAssemblerContext(struct assemblerContext* p);


int processDotString(struct assemblerContext* context, char* args);
int processDotData(struct assemblerContext* context, char* args);
int processDotExtern(struct assemblerContext* context, char* directive,char* args);
int processDotEntry(struct assemblerContext* context,char* directive, char* args);

int processGroup1Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processGroup2Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processGroup3Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processGroup4Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processGroup5Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processGroup6Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processGroup7Command(struct assemblerContext* context, struct commandInfo* cmdInfo, char* args);
int processLine(struct assemblerContext* context, char* line);

int assembler(char* fileName);



#endif /* ASSEMBLER_H_ */
