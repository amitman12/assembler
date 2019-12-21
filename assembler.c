/*
 ============================================================================
 Name        : get_name.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_name.h"
#define NAMECOUNT 30
#define WORDLEN 20
char* names[NAMECOUNT] = { 0 };

/*frees memory*/
void free_names() {
	int i = 0;
	while (names[i] && i < NAMECOUNT)
		free(names[i++]);
}
/* check 2 strings equality. case insensitive. returns 1 for equal and 0 for non equal */
int strcasecmp(char* a, char* b){
	int len = strlen(a);
	int i;
	if(strlen(a)!=strlen(b))
		return 0;
	else{
		for(i=0;i<len;i++){
			if(!((a[i]-b[i])==0||(a[i]-b[i]=='A'-'a')||(a[i]-b[i]=='a'-'A')))
				return 0;
		}
		return 1;
	}

}

/* validate makes sure name[index] is different from all name[j] where j<index */
void validate(int index) {
	int i = 0;
	for (; i < index; i++) {
		if (strcasecmp(names[i], names[index]) == 1) {

			printf("Error, the name %s has already appeared, program will now terminate. \n",names[index]);
			free_names();
			exit(1);
		}
	}
}

/* remove trailing \r and \n from a string */
void chomp(char* str) {
	int len = strlen(str);
	while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
		--len;
	}
	str[len] = '\0';
}

/*randomized a number between 0 and NAMECOUNT-1 and returns that name from global variable names */
char* get_name() {
#define UPPER NAMECOUNT-1
#define LOWER 0
	int i = (rand() % (UPPER - LOWER + 1)) + LOWER;
	return names[i];

}

/* prints the list of given names */
void print_names() {
	int i = 0;
	printf("\nThe names are: \n");
	for (; i < NAMECOUNT; i++)
		printf("%s\n", names[i]);
	printf("\n");
}

int main(void) {

	int i = 0;
	char p[WORDLEN + 2];
	char* randomName;
	for (; i < NAMECOUNT; i++) {
		printf("Please enter a string\n"); /* enter a string*/
		fgets(p, sizeof(p), stdin); /* read string into p */
		printf("The name provided is %s",p);
		chomp(p);
		names[i] = (char*) malloc(WORDLEN + 1);
		strcpy((char*) names[i], p);
		validate(i);
	}

	print_names();
	printf("10 random names from the list: \n");
	for (i = 0; i < 10; i++) {
		randomName = get_name();
		printf("%s\n", randomName);
	}
	free_names();
	return EXIT_SUCCESS;
}
