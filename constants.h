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

#define UNKNOWN_ADDRESS -1
#define OPCODE_COUNT 16
#define MAX_LIZE_SIZE 81
#define REGISTER_COUNT 8
#define INSTS_COUNT 4
#define SYNTAX_ERROR -1
#define LABEL_ALREADY_EXISTS -2
#define LABEL_DOES_NOT_EXIST -333
#define MISMATCH_OPERAND_NUMBER -4
#define RESERVED_WORD -3
#define LABEL_TOO_LONG -5
#define LABEL_EMPTY -6
#define COMMAND_EXPECTED -7
#define COMMAND_DOESNT_EXIST -8
#define ADDRESSING_ERROR -9
#define DOT_STRING 7
#define DOT_DATA 5
#define DOT_EXTERN 7
#define DOT_ENTRY 6
#define EMPTY_STRING -10
#define EMPTY_IMMEDIATE -11
#define ILLEGAL_LABEL -12
#define INVALID_REGISTER -13
#define FIRST_PASS 1
#define SECOND_PASS 2
#define BYTES_IN_WORD 2
#define BITS_IN_WORD 15



#define MAX_LABEL_LEN 31
#define MAX_CMD 1000



extern char* opcodes[];
enum opcodes {mov=0,cmp=1,add=2,sub=3,lea=4,clr=5,not=6,inc=7,dec=8,jmp=9,bne=10,red=11,prn=12,jsr=13,rts=14,stop=15};

enum location {Code = 1, Data = 0, Unknown_Location = -1};
enum type {Unknown_Type = 0, Entry = 1, External = 2, Regular = 3};


enum operand_type {Register = 0, Immediate = 1, Label = 2};
enum addressing_type {Immediate_Addressing=0, Direct_Addressing=1, Indirect_Register_Addressing=2, Direct_Register_Addressing=3};





/*the following functions are used to determine whether a string is a reserved word or not */
int find_in_insts(char*);
int find_in_registers(char*);
int find_in_codes(char*);
int is_reserved_word(char*);


#endif /* CONSTANTS_H_ */
