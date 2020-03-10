/*
 * symboltable.h
 *
 *  Created on: Dec 27, 2019
 *      Author: user
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

/* finds symbol according to label in *(p) */
struct symbol* find_symbol(char* p);

/* adds a given symbol to the table. requires creating a symbol first */
void add_symbol(struct symbol * node);

/* looks for a symbol according to *str, and modifies its address, type and location */
void modify_symbol(char* str, int address, int location, int type);

/* frees memory held up by symboltable */
void dealloc_symbol_table();

/* creates a new object of type symbol, and allocates space in memory for it */
struct symbol* create_symbol(char* name,int address, int type,int location);


/*adds 100 to all addresses of data symbols*/
void update_symbol(int);

#endif /* SYMBOLTABLE_H_ */
