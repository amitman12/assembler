/*
 * utils.h
 *
 *  Created on: Mar 7, 2020
 *      Author: user
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

FILE* fopenFileWithExt(char* fileName, char* mode, char* ext);
char* find_last_quote(char*p);
char* binToOct(signed short int num, char* output);
int is_legal(char* label);
int command_syntax_check(int,char*);
int instruction_syntax_check(int,char*);
void chomp(char*);
int addressing_check_2operands(char*);
char* skipWhiteSpaces(char* );
char* readString(char*,char*,int*);
char* readSymbol(char*, char*);
char* readNumber(char*, int* ,int*);
char* readToken(char*, char* );
char* readRegister(char*, int*);
int isDelimiter(char, char*);
char* strncpyNull(char* dst, char* src, int count);
#endif /* UTILS_H_ */
