/*
 * assembler.h
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

struct commandInfo {
    char* command;
    int (*processCommand)(struct commandInfo*, char* cmd, char* args);
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

int processAssignCommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processCmpCommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processLeaCommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processACommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processBranchCommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processPrnCommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processNonArgsCommand(struct commandInfo* cmdInfo, char* cmd, char* args);
int processLine(int , char* , int* , int* );




#endif /* ASSEMBLER_H_ */
