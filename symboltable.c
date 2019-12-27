#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* returns 1 if two strings are equal, and 0 otherwise. case sensitive */
int eqls(char* a, char* b) {

	if ((*a == '\0') && (*b == '\0')) /*in case both of the strings are empty*/
		return 1;

	while (*a++ == *b++) {
		if (*a == '\0')
			return 1;
	}
	return 0;
}

struct symbol* find_symbol(char* p) {
	struct symbol* temp = table.head;
	while (temp != NULL) {
		if (eqls(p, temp->label))
			return temp;
		temp = temp->next;
	}
	return NULL;
}

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

void edit_symbol(struct symbol* s) {
	struct symbol* temp = table.head;
	while (temp != NULL) {
		if (eqls(s->label, temp->label)) {
			temp->address = s->address;
			temp->location = s->location;
			temp->type = s->type;
			break;
		}
		temp = temp->next;
	}
}

void free_list(struct symbol* s) {
	struct symbol* temp;
	while (s != NULL) {
		temp = s;
		s = s->next;
		free(temp);
	}
}

void dealloc_table() {
	free_list(table.head);
	table.head = NULL;
}

struct symbol* create_symbol(char* name,int address, int type,int location){
	struct symbol* ptr = malloc(sizeof(struct symbol));
	strcpy(ptr->label,name);
	ptr->address = address;
	ptr->location = location;
	ptr->type = type;
	ptr->next = NULL;
	return ptr;
}
