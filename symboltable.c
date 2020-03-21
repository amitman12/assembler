#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"
#include "constants.h"
#include "utils.h"

/*
 static struct symboltable table = {
 NULL
 };
 */

struct symbol* find_symbol(struct symboltable* table, char* p) {
	/* look for label in symboltable returns 0 on failure*/
	struct symbol* temp = table->head;
	while (temp != NULL) {
		if (strcmp(p, temp->label) == 0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

/*if table is empty add given symbol. otherwise loop till end and add it */
void add_symbol(struct symboltable* table, struct symbol * node) {
	if (table->head == NULL) {
		table->head = node;
	} else {
		struct symbol* temp = table->head;
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

/* modifies a symbol whose label is in *(str). if address/locatipn/type arent known yet, no modifications happen */
void modify_symbol(struct symboltable* table, char* str, int address, int type,
		int location) {
	struct symbol* temp = table->head;
	while (temp != NULL) {
		if (strcmp(str, temp->label)==0) {
			if (address != UNKNOWN_ADDRESS)
				temp->address = address;
			if (location != Unknown_Location)
				temp->location = location;
			if (type != Unknown_Type)
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
void dealloc_symbol_table(struct symboltable* table) {
	free_list(table->head);
	table->head = NULL;
}

/* creates a new object of type symbol, and allocates space in memory for it */
struct symbol* create_symbol(struct symboltable* table, char* name, int address,
		int type, int location) {
	struct symbol* ptr = malloc(sizeof(struct symbol));
	strcpy(ptr->label, name);
	ptr->address = address;
	ptr->location = location;
	ptr->type = type;
	ptr->next = NULL;
	return ptr;
}
void update_data_symbol(struct symboltable* table, int instruction_count) {
	struct symbol* node;
	node = table->head;
	while (node != NULL) {
		if (node->location == Data) {
			modify_symbol(table, node->label,
					node->address + 100 + instruction_count, node->type, node->location);
		}
		node = node->next;
	}
}

int checkLabel(struct symboltable* table, char* label) {
	/* returns 1 if label doesn't already exist, and is legal */
	int len;
	len = strlen(label);
	if (len == 0 || len > MAX_LABEL_LEN) {
		return 0;
	}
	if (is_reserved_word(label) == 1) {
		return 0;
	}
	if (find_symbol(table, label) != NULL) {
		return 0;
	}
	return 1;
}


int outEntries(struct symboltable table, char* output) {
	/* copies all .entry info to output file */
	/* returns -1 on fail 0 on success*/
	int count = 0;
	struct symbol* node;
	FILE* out;
	node = table.head;
	while (node) {
		if (node->type == Entry) {
			if (count == 0) {
				out = fopenFileWithExt(output, "w", "ent");
				if (out == NULL) {
					fprintf(stderr,
							"ERROR: could not create/open output file.\n");
					return -1;
				}
			}
			fprintf(out, "%s %d\n", node->label, node->address);
			count++;
		}
		node=node->next;
	}
	if(count>0){
		/*close file*/
		fclose(out);
	}
	return 0;
}
