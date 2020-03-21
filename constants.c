/*
 * constants.c
 *
 *  Created on: Dec 27, 2019
 *      Author: user
 */
#include "constants.h"
char* codes[] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};



char* registers[] = {"r0","r2","r3","r4","r5","r6","r7","r7"};
char* insts[] = {".string",".data",".extern",".entry"};


int is_reserved_word(char* str){
	/* returns 1 if str is a reserved words or 0 if not */
	if(find_in_insts(str)!=-1){
		return 1;
	}
	if(find_in_registers(str)!=-1){
		return 1;
	}
	if(find_in_codes(str)!=-1){
		return 1;
	}

	return 0;
}


int find_in_insts(char* item){
	/* find if a string is an instruction .data or .string or others. return -1 means not found */
	int i;
	for(i=0;i<INSTS_COUNT;i++){
		if(strcmp(item,insts[i])==0)
			return i;
	}
	return -1;
}
int find_in_registers(char* item){
	/* find if a string is a register */
	int i;
	for(i=0;i<REGISTER_COUNT;i++){
		if(strcmp(item,registers[i])==0)
			return i;
	}
	return -1;
}
int find_in_codes(char* item){
	/* find if a string is an opcode */
	int i;
	for(i=0;i<OPCODE_COUNT;i++){
		if(strcmp(item,codes[i])==0)
			return i;
	}
	return -1;
}
