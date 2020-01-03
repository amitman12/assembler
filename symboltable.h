/*
 * symboltable.h
 *
 *  Created on: Dec 27, 2019
 *      Author: user
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
struct symbol {
	char* label;
	int address;
	int type;
	/* type is 1 for external and 0 for entry */
	int location;
	/* location is 1 for code, and 0 for data */
	struct symbol* next;
};

struct symboltable {
	struct symbol *head;
};
struct symboltable table; 
int eqls(char* a, char* b);
struct symbol* find_symbol(char* p);
void add_symbol(struct symbol * node);
void edit_symbol(struct symbol* s);
void dealloc_table();
struct symbol* create_symbol(char* name,int address, int type,int location);
#endif /* SYMBOLTABLE_H_ */
