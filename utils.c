/*
 * utils.c
 *
 *  Created on: Mar 7, 2020
 *      Author: user
 */
#include "utils.h"
#include "constants.h"
#include "symboltable.h"

int checkLabel(char* label){
	/* returns 1 if label doesn't already exist, and is legal */
	int len;
	len = strlen(label);
	if(len==0||len>MAX_LABEL){
		return 0;
	}
	if(is_reserved_word(label)==1){
		return 0;
	}
	if(find_symbol(label)!=NULL){
		return 0;
	}
	return 1;
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
	return p;
}



char* readSymbol(char*p, char* str) {
	/* puts label into given str*/
	/*returns pointer to comma at end of symbol. or pointer to end of string if none exist. NULL on fail*/
	char* label;
	char*start;
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
        return NULL;
    }
    if(!isalpha(*p)||*p=='8'||*p=='9'){
    	/* expecting valid register name */
    	return NULL;
    }

    *reg = (int)*p;
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



char* readNumber(char*p, int* number) {
	/* puts immediate in number */
	/* returns NULL on error, returns pointer to comma at end of operand if there is one */
	char*start;
	char*end;
	char* str;
	start = p;
	if(*p=='+'||*p=='-'){
		p++;
	}

	if(*p=='\0'||!isdigit(*p)){
		printf("expecting immediate");
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
		return p;
	}
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



