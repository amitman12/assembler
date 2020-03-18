/*
 * utils.c
 *
 *  Created on: Mar 7, 2020
 *      Author: user
 */
#include <ctype.h>
#include "utils.h"
#include "constants.h"
#include "symboltable.h"

FILE* fopenFileWithExt(char* fileName, char* mode, char* ext) {
	/* open file with given extension */
    char fileNameWithExt [FILENAME_MAX];
    sprintf(fileNameWithExt, "%s.%s", fileName, ext);
    return fopen(fileNameWithExt, mode);
}

unsigned int twosComplement(int x) {
    return (x >= 0) ? x : ((~((unsigned int)-x) & 0x7fff) + 1);
}

signed short int swapBits(signed short int num){
	/*swapBits gets num and returns a number with swapped bits */
	signed short int result;
	int i;
	result = 0;
	for(i=0;i<BITS_IN_WORD;i++){
		result = result + ((num&1)^1);
		result = result<<1;
		num = num<<1;
	}
	return result;
}




/* remove trailing \r and \n from a string */
void chomp(char* str) {
	int len = strlen(str);
	while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
		--len;
	}
	str[len] = '\0';
}



char* find_last_quote(char*p){
	/* returns last quote in a string */
	char* last;
	while(*p!='\0'){
		if(*p=='\"'){
			last = p;
		}
		p++;
	}
	return last;
}

char* readString(char*p, char*str, int* len){
	/* start - p points to first argument in string */
	/* end - p points to end of line */
	/* puts string in str */
	/* puts the number of characters in the string in len, including \0 */
	int count = 0;
	char* end;
	char* start;
	end = find_last_quote(p);
	p = skipWhiteSpaces(p);
	if(*p=='\0'){
		*len = count;
		return p;
	}

	if(*p!='\"'){
		/* illegal string */
		*len = -1;
		return NULL;
	}
	p++;
	start = p;
	while(p<end&&isprint(*p)){
		count++;
		p++;
	}
	if(p<end){
		/* illegal string */
		*len = SYNTAX_ERROR;
		return NULL;
	}
	if(count==0){
		/* empty string */
		*len = EMPTY_STRING;
		return NULL;
	}
	p++;
	p = skipWhiteSpaces(p);
	if(*p=='\0'){
		/*legal string */
		/*put len of string in len*/
		/*copies string to str*/
		*len = count+1;
		strncpy(str,start,count);
		return p;
	}
	return NULL;
}

char* readSymbol(char*p, char* str) {
	/* puts label into given str*/
	/*returns pointer to comma at end of symbol. or pointer to end of string if none exist. NULL on fail*/
	char* start;
	char*end;
	p = skipWhiteSpaces(p);
    if (!isalpha(*p)) {
        return NULL;
    }
    start = p;
    ++p;
    for(; isalnum(*p); ++p) {
    }
    end = p;
    strncpy(str,start,end-start);
    p = skipWhiteSpaces(p);
    if (*p == '\0') {
        return p;
    }
    if(*p==','){
    	return p;
    }
    return NULL;
}

char* readRegister(char*p, int* reg) {
	/* copies register name into reg*/
	/* returns pointer to comma at end of operand if there is one, returns NULL on failure*/
    if ((*p)!='r') {
    	/* expecting register name */
    	*reg = INVALID_REGISTER;
        return NULL;
    }
    p++;
    if(*p<'0'||*p>'7'){
    	/* expecting valid register name */
    	*reg = INVALID_REGISTER;
    	return NULL;
    }
    *reg = *p - '0';
    p++;
    p = skipWhiteSpaces(p);


    if (*p == '\0') {
    	/*done*/
        return p;
    }
    if(*p==','){
    	return p;
    }
    return NULL;
}



char* readNumber(char*p, int* number, int* err_type) {
	/* puts immediate in number */
	/* puts error type in err_type 0 means success*/
	/* returns NULL on error, returns pointer to comma at end of operand if there is one */
	char*start;
	char*end;
	p = skipWhiteSpaces(p);
	start = p;
	if(*p=='+'||*p=='-'){
		p++;
	}

	if(*p=='\0'||!isdigit(*p)){
		*err_type = EMPTY_IMMEDIATE;
		return NULL;
	}
	while(isdigit(*p)){
		++p;
	}
	end = p;

	/* create a string of the number then store in given address*/
	*number = (int)strtol(start,&end,10);

	p = skipWhiteSpaces(p);
	if(*p==','||*p=='\0'){
		*err_type=0;
		return p;
	}

	*err_type=SYNTAX_ERROR;
	return NULL;
}


char* readToken(char*p, char* delimiters) {
	/* returns pointer to first character after token - the delimiter */
    for (; *p; ++p) {
        if (isDelimiter(*p, delimiters)) {
            return p;
        }
    }
    return p;
}

int is_legal(char* label){
	/* -1 on illegal label and 0 on legal */
	if(!isalpha(*label)){
		return SYNTAX_ERROR;
	}
	while(isalnum(*label)){
		label++;
	}
	if(*label!='\0'){
		return SYNTAX_ERROR;
	}
	return 0;
}

char* skipWhiteSpaces(char* p) {
	/*increments p to skip tabs and spaces */
    while (*p == ' ' || *p == '\t') {
        ++p;
    }
    return p;
}

int isDelimiter(char ch, char* delimiters) {
	/*returns 1 if ch is in delimiters and 0 otherwise */
    char *p;
    for (p = delimiters; *p; ++p) {
        if (*p == ch) {
            return 1;
        }
    }
    return 0;
}



char* strncpyNull(char* dst, char* src, int count) {
    strncpy(dst, src, count);
    dst[count] = '\0';
    return dst;
}
