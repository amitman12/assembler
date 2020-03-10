/*
 * constants.h
 *
 *  Created on: Dec 27, 2019
 *      Author: user
 */

#ifndef CONSTANTS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONSTANTS_H_
#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define INDIRECT_REGISTER_ADDRESSING 2
#define DIRECT_REGISTER_ADDRESSING 3
#define MAX_LABEL_LENGTH 50
#define MAX_CHARACTERS_IN_COMMAND 1000
#define UNKNOWN_ADDRESS -1
#define OPCODE_COUNT 16
#define MAX_LIZE_SIZE 81
#define REGISTER_COUNT 8
#define INSTS_COUNT 4
#define DOT_ENTRY 1
#define DOT_STRING 2
#define DOT_DATA 3
#define DOT_EXTERN 4
#define SYNTAX_ERROR -1
#define LABEL_ALREADY_DEFINED -2
#define RESERVED_WORD -3

extern char* opcodes[];
enum opcodes {mov=0,cmp=1,add=2,sub=3,lea=4,clr=5,not=6,inc=7,dec=8,jmp=9,bne=10,red=11,prn=12,jsr=13,rts=14,stop=15};
enum location {Code = 1, Data = 0, Unknown_Location = -1};
enum type {External = 1, Entry = 0, Unknown_Type = -1,Regular = 2};


/*the following 3 functions are used to determine whether a label is a reserved word or not */
int find_in_insts(char*);
int find_in_registers(char*);
int find_in_codes(char*);



#endif /* CONSTANTS_H_ */
