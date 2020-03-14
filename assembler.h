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
    int (*processCommand)(struct assemblerContext* context, struct commandInfo*, char* cmd, char* args, int pass);
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


int processDotString(struct assemblerContext* context, char* args, int pass);
int processDotData(struct assemblerContext* context, char* args, int pass);
int processDotExtern(struct assemblerContext* context, char* directive,char* args, int pass);
int processDotEntry(struct assemblerContext* context,char* directive, char* args, int pass);

int processAssignCommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processCmpCommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processLeaCommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processACommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processBranchCommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processPrnCommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processNonArgsCommand(struct assemblerContext* context, struct commandInfo* cmdInfo, char* cmd, char* args, int pass);
int processLine(struct assemblerContext* context, char* line, int pass);

int assembler(char* fileName);



#endif /* ASSEMBLER_H_ */
