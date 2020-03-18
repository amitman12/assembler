/*
 * symboltable.h
 *
 *  Created on: Dec 27, 2019
 *      Author: user
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "constants.h"

struct symbol {
    char label[MAX_LABEL];
    int address;
    /* type is enum in constants.h */
    int type;
    /* location is enum in constants.h */
    int location;
    struct symbol* next;
};

struct symboltable {
    struct symbol *head;
};

/* finds symbol according to label in *(p) */
struct symbol* find_symbol(struct symboltable* table, char* p);

/* adds a given symbol to the table. requires creating a symbol first */
void add_symbol(struct symboltable* table, struct symbol * node);

/* looks for a symbol according to *str, and modifies its address, type and location */
void modify_symbol(struct symboltable* table, char* str, int address,  int type,int location);

/* frees memory held up by symboltable */
void dealloc_symbol_table(struct symboltable* table);

/* creates a new object of type symbol, and allocates space in memory for it */
struct symbol* create_symbol(struct symboltable* table, char* name,int address, int type,int location);


/*adds IC+100 to all addresses of data symbols*/
void update_data_symbol(struct symboltable* table, int);

int checkLabel(struct symboltable* table, char* label);

int outEntries(struct symboltable table, char* output);
int outExterns(struct symboltable table, char* output);



#endif /* SYMBOLTABLE_H_ */
