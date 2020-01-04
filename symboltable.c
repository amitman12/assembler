#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"
#include "constants.h"

struct symbol {
	char label[MAX_LABEL_LENGTH];
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
static struct symboltable table = {
	NULL
};


struct symbol* find_symbol(char* p) {
	struct symbol* temp = table.head;
	while (temp != NULL) {
		if (strcmp(p, temp->label))
			return temp;
		temp = temp->next;
	}
	return NULL;
}

/*if table is empty add given symbol. otherwise loop till end and add it */
void add_symbol(struct symbol * node) {
	if (table.head == NULL) {
		table.head = node;
	} else {
		struct symbol* temp = table.head;
		while (temp != NULL) {
			if (temp->next != NULL)
				temp = temp->next;
			else {
				temp->next = node;
				break;
			}
		}
	}
}

/* modifies a symbol whose label is in *(str). if address/locatipn/type arent known yet, no modification happens */
void modify_symbol(char* str, int address, int location, int type) {
	struct symbol* temp = table.head;
	while (temp != NULL) {
		if (strcmp(str, temp->label)) {
			if(address!= -1)
				temp->address = address;
			if (location!= Unknown_Location)
				temp->location = location;
			if(type!= Unknown_Type)
				temp->type = type;
			break;
		}
		temp = temp->next;
	}
}

/* frees every symbol in the symboltable by looping over the symbols */
static void free_list(struct symbol* s) {
	struct symbol* temp;
	while (s != NULL) {
		temp = s;
		s = s->next;
		free(temp);
	}
}

/* calls free_list with head symbol in order to free the table */
void dealloc_symbol_table() {
	free_list(table.head);
	table.head = NULL;
}

/* creates a new object of type symbol, and allocates space in memory for it */
struct symbol* create_symbol(char* name,int address, int type,int location){
	struct symbol* ptr = malloc(sizeof(struct symbol));
	strcpy(ptr->label,name);
	ptr->address = address;
	ptr->location = location;
	ptr->type = type;
	ptr->next = NULL;
	return ptr;
}
