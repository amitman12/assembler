/*
 * firstpass.c
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */
#include "constants.h"
#include "utils.h"
#include <errno.h>
#include <ctype.h>
#define min(X,Y) (((X) <(Y))?(X):(Y))

int count_variables_in_instruction(char* line){
	/*returns number of variables in instruction line. excluding \0 in .string */
	/* count starts from one, each comma means one additional variable */
	char* unlabeled_line;
	int count = 1;
	int i;
	strcpy(unlabeled_line,line+is_new_label(line));
	for(i = 0;i<strlen(unlabeled_line);i++){
		if(unlabeled_line[i]== ','){
			count++;
		}
	}
	return count;
}

int is_new_label(char* new_label, char* line){
	/*returns pointer to first char after label. e.x if line is - >  Label: mov r1,r2  output will be 6. return 0 means no label. -1 means syntax error*/
	/* new_label is pointer to string. if found, defined label will be put there */
	int loc = 0;
	int len = strlen(line);
	int start;
	int end;
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	/* allow spaces and tabs */

	start = loc;
	if(isalpha(line)==0){
		return -1;
		/*label/first word has to start with a letter - this is a syntax error */
	}
	else{
		for(;loc<min(MAX_LABEL_LENGTH,strlen(line+loc));loc++){
			if(isalpha(line+loc)==0 && isdigit(line+loc)==0){
				if((*(line+loc)== ':')&&loc<len){
					end = loc-1;
					strncpy(new_label,line+start,end-start);
					if(find_symbol(new_label)!=0){
						/*label defined more than once - error*/
						return LABEL_ALREADY_DEFINED;
					}
					if(find_in_codes(new_label)!=0||find_in_registers(new_label)!=0||find_in_insts(new_label)!=0){
					/*error- label found. label is reserved word.*/
						return RESERVED_WORD;
					}
					/*new label doesn't exist and isn't reserved - add to symboltable */
				}

				else{
					/* no label in this line */
					return 0;
				}
			}
		}
		return 0;
	}
}


int is_comment_line(char* line){
	/*returns 1 for comment 0 for not comment */
	if(*(line) == ';'){
		return 1;
		/*comment line*/
	}
	return 0;
}
int is_empty_line(char* line){
	/* returns 1 for empty 0 for not*/
	int index;
	for(index = 0;index<strlen(line);index++){
		if(*(line+index)!= '\t'&&*(line+index)!= '\n'&&*(line+index)!= ' '){
			return 0;
		}
	}
	return 1;
}

int is_command_line(char* line){
	/* returns the amount of words in machine code needed for this command line. */
	/* returns 0 if not command line */
	/*returns negatives on syntax errors */
	char* unlabeled_line;
	char* label;
	char delimiters[] = " \t";
	char* first_word;
	int command_type;
	int flag;
	label = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	unlabeled_line = (char*)malloc(sizeof(char)*MAX_CHARACTERS_IN_COMMAND - MAX_LABEL_LENGTH);
	flag = is_new_label(label, line);
	if(flag<0){
		free(unlabeled_line);
		free(label);
		return flag;
		/*errors*/
	}

	strcpy(unlabeled_line,line+flag);
	first_word = strtok(unlabeled_line,delimiters); /*gets first word in line that isn't a label*/
	command_type = find_in_codes(first_word);
	if (command_type==-1){
		/* not a command line */
		free(unlabeled_line);
		free(label);
		return 0;
	}
	else{
		if(command_type < 5 && (command_syntax_check(1,line+flag)!=SYNTAX_ERROR)){
			/* in this case we have two operands and the command word */
			/* addressing is checked in command_syntax_check */
			free(unlabeled_line);
			free(label);
			return 3;
		}
		if(command_type>4 && command_type<14 && command_syntax_check(2,line+flag)!=SYNTAX_ERROR){
			/*in this case we have one operand and the command word  */
			/* addressing is checked in command_syntax_check */
			free(unlabeled_line);
			free(label);
			return 2;
		}
		if(command_syntax_check(3,line+flag)!=SYNTAX_ERROR){
			/* check addressing and figure out how many words are needed. but in this case we have 0 operands and the command word  */
			free(unlabeled_line);
			free(label);
			return 1;
		}
	}
	free(unlabeled_line);
	free(label);
	return -1; /*syntax error*/
}

int is_instruction_line(char* line){
	/* returns 0 if not an instruction (.data or .string or .entry or .external) otherwise, returns 1,2,3,4 for entry,string,data,external */
	/*returns negatives defined in constants  on errors */
	char* label;
	char* unlabeled_line;
	int instruction_type;
	char delimiters[] = " \t";
	char* first_word;
	int flag;
	label = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	unlabeled_line = (char*)malloc(sizeof(char)*(MAX_CHARACTERS_IN_COMMAND - MAX_LABEL_LENGTH));
	flag = is_new_label(label,line);
	if(flag<0){
		free(label);
		free(unlabeled_line);
		return flag;
		/*errors*/
	}

	strcpy(unlabeled_line,line+flag);
	first_word = strtok(unlabeled_line,delimiters); /*gets first word in line that isn't a label*/
	instruction_type =  find_in_insts(first_word);
	if(instruction_type==-1){
		/*not an instruction */
		free(label);
		free(unlabeled_line);
		return 0;
	}
	else{
		if(instruction_type==DOT_ENTRY && instruction_syntax_check(DOT_ENTRY,line+flag)!=SYNTAX_ERROR){
			free(label);
			free(unlabeled_line);
			return DOT_ENTRY;
		}
		if(instruction_type==DOT_STRING && instruction_syntax_check(DOT_STRING,line+flag)!=SYNTAX_ERROR){
			free(label);
			free(unlabeled_line);
			return DOT_STRING;
		}
		if(instruction_type==DOT_DATA && instruction_syntax_check(DOT_DATA,line+flag)!=SYNTAX_ERROR){
			free(label);
			free(unlabeled_line);
			return DOT_DATA;
		}
		if(instruction_type==DOT_EXTERN && instruction_syntax_check(DOT_EXTERN,line+flag)!=SYNTAX_ERROR){
			free(label);
			free(unlabeled_line);
			return DOT_EXTERN;
		}

	}

	free(label);
	free(unlabeled_line);
	return SYNTAX_ERROR;
}


int line_parse(int* ic, int* dc, char* line){
	/* increments instruction count, data count when needed. adds newly defined labels to symboltable. checks syntax*/
	/* ic is pointer to instruction count in firstpass*/
	/*dc is pointer to data count in firstpass*/
	/* returns 0 on success, and negative numbers defined in constants.h for errors */
	int words;
	int type;
	char* label= (char*)malloc(sizeof(char*)*MAX_LABEL_LENGTH);
	int flag = is_new_label(label,line);
	if(is_empty_line(line)==1 || is_comment_line(line)==1){
		/*data count and instruction count are unchanged */
		/* no new label found */
		free(label);
		return 0;
	}

	words = is_command_line(line);
	if(words>0){
		/*this is a command line*/
		if(flag>0){
			/*new label defined in this line, copy label name without : to variable label. */
			add_symbol(create_symbol(label, *(ic)+100, Regular, Code));
			/* still need to create binary code of the first word and all other "info words" with an operand that is not a label*/
			/* increment ic by the amount of words needed in machine code */
			*(ic) = *(ic)+ words;
			free(label);
			return 0;
		}
		else{
			if(flag==0){
			/*no new label defined in this line */
			/* still need to create binary code of the first word and all other "info words" with an operand that is not a label*/
			/* increment ic by the amount of words needed in machine code */
			*(ic) = *(ic)+ words;
			free(label);
			return 0;
		}
		/* returns errors */
		free(label);
		return flag;
		}
	}

	type = is_instruction_line(line);
	if(type>0){
		/* this is an instruction line*/
		if(type==DOT_EXTERN){
			/* check how many operands extern line has and add them all (with no address) to symboltable assuming theyre all new.*/
			free(label);
		}
		if(type==DOT_STRING){
			/*.string */
			if(flag>0){
				/*new label*/
				add_symbol(create_symbol(*(label), *(dc), Regular, Data));
				*(dc) = *(dc)+ count_variables_in_instruction(line) + 1;
				free(label);
				return 0;
			}
			else{
				if (flag==0){
					/*no new label - increment dc*/
					*(dc) = *(dc)+ count_variables_in_instruction(line) + 1;
					free(label);
					return 0;
				}
				free(label);
				return flag;
				/*returns errors*/
		}
	}
		if(type==DOT_DATA){
			/*.data line */
			if(flag>0){
				/*new label*/
				add_symbol(create_symbol(*(label), *(dc), Regular, Data));
				*(dc) = *(dc)+ count_variables_in_instruction(line);
				free(label);
				return 0;
			}
			else{
				if(flag==0){
					/*no new label - increment dc*/
					*(dc) = *(dc)+ count_variables_in_instruction(line);
					free(label);
					return 0;
				}
				/*return errors */
				free(label);
				return flag;
				}
			}
	}
	free(label);
	return -1;
}


int firstPass(char* fileName) {
	/* i assumed firstPass shouldn't code anything, and leave the coding(translation to machine code) to second pass */
	/* returns 0 on success*/
	/* returns negative numbers defined in constants.h on errors*/
    FILE* inputFile;
    char* line;
    int instruction_count = 0;
    int data_count = 0;
    int* ic = &instruction_count;
    int* dc = &data_count;
    int result;
    inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "could not open file %s. error:%s\n", fileName, strerror(errno));
        return 1;
    }
    /*CAUTION. must not return until end of function. must call fclose*/

	line = malloc(MAX_CHARACTERS_IN_COMMAND);
    while (fgets(line, MAX_CHARACTERS_IN_COMMAND, inputFile)) {
    	result = line_parse(ic,dc,line);
    	if(result<0)
    		/* result is either LABEL_ALEADY_DEFINED or SYNTAX_ERROR or RESERVED_WORD */
    		return result;
    }
    /*if there were no syntax errors - continue*/
    update_symbol(instruction_count);
    free(line);
    fclose(inputFile);
    return 0;
}

