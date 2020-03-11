/*
 * utils.h
 *
 *  Created on: Mar 7, 2020
 *      Author: user
 */

#ifndef UTILS_H_
#define UTILS_H_


int checkLabel(char* label);

char* find_last_quote(char*p);

int command_syntax_check(int,char*);
int instruction_syntax_check(int,char*);

int addressing_check_2operands(char*);
char* skipWhiteSpaces(char* );
char* readSymbol(char*, char*);
char* readNumber(char*, int* );
char* readToken(char*, char* );
char* readRegister(char*, int*);
int isDelimiter(char, char*);
char* strncpyNull(char* dst, char* src, int count);
#endif /* UTILS_H_ */
