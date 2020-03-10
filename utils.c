/*
 * utils.c
 *
 *  Created on: Mar 7, 2020
 *      Author: user
 */
#include "utils.h"
#include "constants.h"


int illegal_dest_addressing(int op_type,int addressing){

	/*returns 1 on illegal addressing 0 otherwise */
	if(addressing==-1){
		return 1;
	}
	if(op_type ==0||op_type ==2||op_type ==3||op_type ==4||op_type ==5||op_type ==6||op_type ==7||op_type ==8||op_type ==11){
		if(addressing != DIRECT_REGISTER_ADDRESSING){
			return 0;
		}
		return 1;
	}
	if(op_type==1||op_type==12){
		/*these commands can have any addressing */
		return 0;
	}
	if(op_type==9||op_type==10||op_type==13){
		if(addressing!=DIRECT_REGISTER_ADDRESSING && addressing!=IMMEDIATE_ADDRESSING){
			return 0;
		}
		return 1;
	}
	return 1;
}

int illegal_src_addressing(int op_type,int addressing){

	/*returns 1 on illegal addressing 0 otherwise */
	if(addressing==-1){
		/*these commands can have any addressing */
		return 1;
	}
	if(op_type ==0||op_type ==1||op_type ==2||op_type ==3)
			return 0;

	if(op_type==4){
		if(addressing==DIRECT_ADDRESSING)
			return 0;
		return 1;
	}
	return 0;
}




int inst_parse_operand(char* line){
	/* gets pointer to first char of operand, checks syntax of operand.*/
	/* returns pointer to next operand, or 0 if none exist */
	/* returns -1 on syntax error */
	int loc =0;
	int len = strlen(line);
	if(loc==len)
		return 0;
	if((line[loc]=='+'||line[loc]=='-')){
		if(loc+1<len){
			loc++;
			if(!isdigit(line[loc])){
				return SYNTAX_ERROR;
				/* after sign a digit is expected */
			}
			while(isdigit(line[loc])&&loc<len){
				loc++;
			}
			while((line[loc]==' '||line[loc]=='\t')&&loc<len){
				loc++;
			}
			if(loc==len){
				return 0;
			}
			if(line[loc]==','){
				loc++;
				if(loc==len)
					return SYNTAX_ERROR;
					/* a comma can't be a start of an operand */
				else
					return loc;
			}
			return SYNTAX_ERROR;
		}
		else
			/* + or - can't be the last character in an operand */
			return SYNTAX_ERROR;
	}
	else{
		/*number doesn't start with a sign */
		if(!isdigit(line[loc])){
			return SYNTAX_ERROR;
			/* first character has to be a digit */
			}
			while(isdigit(line[loc])&&loc<len){
				loc++;
			}
			while((line[loc]==' '||line[loc]=='\t')&&loc<len){
				loc++;
			}
			if(loc==len){
				return 0;
			}
			if(line[loc]==','){
				loc++;
				if(loc==len)
					return SYNTAX_ERROR;
					/* a comma can't be the end of a line */
				else
					return loc;
				}
			return SYNTAX_ERROR;
	}
}

int inst_label_parse_operand(char* current, char* line){
	/* gets pointer to char in first operand */
	/*returns -1 on syntax error, or the location of next operand in line. */
	/* return 0 means no next operand */
	/* puts current operand in current */
	int loc = 0;
	int len = strlen(line);
	if(!isalpha(line[loc])){
		return SYNTAX_ERROR;
		/* label has to start with an alphabetic character*/
	}
	while((isalpha(line[loc])||isdigit(line[loc]))&&loc<len){
		loc++;
	}
	strncpy(current,line,loc);
	while((line[loc]==' '||line[loc]=='\t')&&loc<len){
		loc++;
	}
	if(loc==len)
		return 0;
	else{
		if((line[loc]==',')&&loc+1<len){
			/* comma isn't the last character */
			loc++;
			return loc;
		}
		return SYNTAX_ERROR;
	}
}

int instruction_syntax_check(int type,char* line){
	/*checks syntax of an instruction line. type is instruction type. DOT_ENTRY, DOT_STRING, DOT_DATA, DOT_EXTERN*/
	/*returns -1 on syntax error */
	/*given line has no new label - it has been removed prior to calling this function */
	int loc = 0;
	int len =strlen(line);
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	while(line[loc]=='.'||isalpha(line[loc])){

		loc++;
	}
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	/* these 3 whiles increment line to point to the first char of the first operand */
	/* ex if the line was(including spaces):     .data     8,-7,34 line now points to '8' */
	/*the second while does not check syntax correctly - ex .dat.a is accepted but the first word was already scanned and checked in is_instruction_line using strtok*/
	if(type==DOT_DATA)
		return dot_data_syntax_check(line+loc);
	if(type==DOT_STRING)
		return dot_string_syntax_check(line+loc);
	if(type==DOT_EXTERN)
		return dot_extern_entry_syntax_check(type, line+loc);
	if(type==DOT_ENTRY)
		return dot_extern_entry_syntax_check(type, line+loc);

	return -1;
}

int dot_data_syntax_check(char* line){
	/* returns the number of numbers in line on correct syntax, -1 on syntax error */
	/*line points to first character of first operand */
	int count = 0;
	int loc = 0;
	int len = strlen(line);
	while(loc<len){
		loc = inst_parse_operand(line+loc);
		if(loc==SYNTAX_ERROR){
			return SYNTAX_ERROR;
		}
		if(loc==0){
			/*this means inst_parse_operand returned 0 */
			if(count==0)
				/*.data line with no numbers is a syntax error */
				return SYNTAX_ERROR;
			else
				return count;
		}
		count++;
	}
}

int dot_string_syntax_check(char* line){
	/* returns the size of string in .string line, excluding " */
	/* returns -1 on syntax error */
	/*line points to first character of first operand */
	int loc = 0;
	int len = strlen(line);
	int count=0;
	if(loc==len){
		return SYNTAX_ERROR;
		/*.string line with no string is a syntax error */
	}
	/* '\"' ? or'"' */
	if(line[loc]!='\"'){
		return SYNTAX_ERROR;
		/* string has to start with " */
	}
	/* characters are printable ascii characters */
	while(isprint(line[loc])&&loc<len){
		++count;
	}
	if(!isprint(line[loc]))
		return SYNTAX_ERROR;
		/* illegal string */
	if(line[loc]=='\"'){
		return count;
	}
	return SYNTAX_ERROR;
}

int dot_extern_entry_syntax_check(int type, char* line){
	/* operands of .extern or .entry lines are labels */
	/* check that the labels are allowed */
	/* a new label defined in a .extern or .entry line is useless */
	/* returns -1 on syntax error */
	/* returns number of operands (labels) in .extern or .entry line */
	/* line points to first char of first operand */
	/* if .extern - adds labels to symboltable with extern property */
	int count = 0;
	int loc = 0;
	int len = strlen(line);
	char* current;
	current = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	while(loc<len){
		loc = inst_label_parse_operand(current,line+loc);
		if(loc==SYNTAX_ERROR){
			/* inst_label_parse_operand returned syntax error */
			free(current);
			return SYNTAX_ERROR;
		}
		if(loc==0&&count==0){
			/* .extern or .entry line with no operands */
			free(current);
			return SYNTAX_ERROR;

		}
		count++;
		if(type==DOT_EXTERN){
			/*add label to symboltable - without checking if already exists as described in algorithm */
			add_symbol(create_symbol(current,UNKNOWN_ADDRESS,External,Unknown_Location));
		}
		if(loc==0){
			/*end of line - no new operands*/
			free(current);
			return count;
		}
	}
	return count;
}




int command_syntax_check_type1(char* line){
	/* given line has no new label definition - it has been removed prior to this */
	/* checks syntax in a command line that has 2 operands */
	/* returns -1 on syntax error. otherwise returns the amount of words in machine code required, done by checking addressing with parse_operand */
	int addressing_1;
	int addressing_2;
	int* adrs1 = &addressing_1;
	int* adrs2 = &addressing_2;

	int command_start;
	int command_end;
	int command_type;
	char* command;

	int operand1_type;
	int operand2_type;
	int start1; /*start index of first operand */
	int start2; /*start index of second operand */
	int end1; /* end index of first operand*/
	int end2; /* end index of second operand*/

	int len = strlen(line);
	int loc = 0;

	/* following 3 whiles advances loc so that it points to beginning of first operand including * and # */
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	command_start = loc;

	while((isalpha(line[loc]))&&loc<len){
		loc++;
	}
	command_end = loc;
	strncpy(command,line+command_start,command_end-command_start);
	command_type = find_in_codes(command);
	if(command_type>4||command_type==-1){
		/* this command either does not exist or has different than 2 operands */
		return SYNTAX_ERROR;
	}

	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	start1 = loc;
	start2 = command_parse_operand(line+loc, adrs1);
	end1 = start2-2;
	end2 = command_parse_operand(line+start1, adrs2);
	if(addressing_1==SYNTAX_ERROR||addressing_2==SYNTAX_ERROR||illegal_dest_addressing(command_type,addressing_1)||illegal_src_addressing(command_type,addressing_2)||end2!=len){
		/*parse_operand returned an error or there were more than 2 operands/similar syntax error or illegal addressing*/
		return -1;
	}
	else{
		if((addressing_1==DIRECT_REGISTER_ADDRESSING||addressing_1==INDIRECT_REGISTER_ADDRESSING)&&(addressing_2==DIRECT_REGISTER_ADDRESSING||addressing_2==INDIRECT_REGISTER_ADDRESSING)){
			return 2;
			/* in this situation we only need 2 words in machine code : 1 for command and 1 for both operands */
		}
		return 3;
		/* here we need 1 word in machine code per operand, plus one word for the command */
	}
}

int command_syntax_check_type2(char* line){
	/* given line has no new label definition - it has been removed prior to this */
	/* checks syntax in a command line that has 1 operand */
	/*operand here is always a destination operand */
	/* returns -1 on syntax error. otherwise returns the amount of words in machine code required, done by checking addressing with parse_operand */
	int addressing_1;
	int* adrs1 = &addressing_1;
	int loc = 0;
	char* command;
	int command_start;
	int command_end;
	int operand_end;
	int command_type;
	int operand_start; /*index of first operand */
	int len = strlen(line);

	/* following 3 whiles advances loc so that it points to beginning of first operand including * and # */
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	command_start = loc;

	while((isalpha(line[loc]))&&loc<len){
		loc++;
	}
	command_end = loc;
	strncpy(command, line+command_start, command_end-command_start);
	command_type = find_in_codes(command);
	if(command_type<5 ||command_type>13){
		return SYNTAX_ERROR;
		/* either unknown command or this command has 0 operands or this command has 2 operands */
	}

	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	operand_start = loc;
	operand_end = command_parse_operand(line+loc, adrs1);
	if(addressing_1==SYNTAX_ERROR||operand_end!=len||illegal_dest_addressing(command_type,addressing_1)==1){
		/*parse_operand returned an error, or there is more than 1 operand in a command that requires only one. or illegal addressing */
		return -1;
	}
	else{
			return 2;
			/* we need 2 words in machine code. 1 for command and 1 for operand, regardless of addressing type */
		}
	}

int command_syntax_check_type3(char* line){
	/* given line has no new label definition - it has been removed prior to this */
	/* checks syntax in a command line that has 0 operands - has to be rts or stop command */
	/* returns -1 on syntax error. otherwise returns the amount of words in machine code required */
	int loc = 0;
	int end;
	int len = strlen(line);
	int start;

	/* following 3 whiles advances loc so that it points to beginning of first operand including * and #. since there are no operands it points to end of line. */
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	start = loc;
	while((isalpha(line[loc]))&&loc<len){
		loc++;
	}
	end = loc;

	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	if(loc==len && (strncmp(line+start,"stop",end-start)==0||strncmp(line+start,"rts",end-start)==0)){
		/* we only need 1 word in machine code for rts or stop commands */
		return 1;
	}
	return SYNTAX_ERROR;
}

int command_parse_operand(char* line, int* adrs){
	/* line is pointer to first operand */
	/* puts addressing of operand or syntax error in adrs */
	/*returns location of start of next operand or -1 if parsing resulted in error*/
	int loc = 0;
	char* operand;
	int start;
	int len = strlen(line);
	while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
		loc++;
	}
	if((!isalpha(line[loc]))&&(line[loc]!='#')&&(line[loc]!='*')){
		/*operand in command has to start with a letter or # or * after spaces */
		*(adrs) = SYNTAX_ERROR;
		return SYNTAX_ERROR;
	}
	if(line[loc]=='#'&& loc<len){
		loc++;

		while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
			loc++;
		}

		while((isdigit(line[loc]))&&loc<len){
			loc++;
		}
		while((line[loc]==' '||line[loc]== '\t')&&loc<=len){
			loc++;
		}
		if(line[loc]!=',' && line[loc]!='\0'){
			*(adrs) = SYNTAX_ERROR;
			return SYNTAX_ERROR;
		}
		*(adrs) = IMMEDIATE_ADDRESSING;
		if(loc==len){
			/*operand end is end of line*/
			return loc;
		}
		if(line[loc] == ',' && loc+1<len){
			return loc+1;
		}
		*(adrs) = SYNTAX_ERROR;
		return SYNTAX_ERROR;
	}
	if(line[loc]=='*'&& loc+1<len){
		loc++;

		while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
			loc++;
		}

		if(line[loc]!='r' || loc<len){
			*(adrs) = SYNTAX_ERROR;
			return SYNTAX_ERROR;
		}
		loc++;
		if(isdigit(line[loc])&&(line[loc]!='9')&&(line[loc]!=8)){
			while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
				loc++;
			}

			if(loc==len){
				*(adrs) = INDIRECT_REGISTER_ADDRESSING;
				return loc;
			}
			if(line[loc]==',' && loc<len){
				*(adrs) = INDIRECT_REGISTER_ADDRESSING;
				return loc+1;
			}
		}
		*(adrs) = SYNTAX_ERROR;
		return SYNTAX_ERROR;
	}
	if(isalpha(line[loc])&&loc<len){
		start = loc;
		while((isalpha(line[loc])||isdigit(line[loc]))&&loc<len){
			loc++;
		}
		strncpy(operand,line+start,loc-start);
		while((line[loc]==' ' ||line[loc]=='\t')&&loc<len){
			loc++;
		}
		if(loc==len){
			if(find_in_registers(operand)){
				*(adrs) = DIRECT_REGISTER_ADDRESSING;
				return loc;
			}
			else{
				*(adrs) = DIRECT_ADDRESSING;
				return loc;
			}
		}

		else{
			if(loc<len){
				if(line[loc]==','){
					if(find_in_registers(operand)){
						*(adrs) = DIRECT_REGISTER_ADDRESSING;
						return loc+1;
					}
					else{
						*(adrs) = DIRECT_ADDRESSING;
						return loc+1;
					}
				}
			}
			*(adrs) = SYNTAX_ERROR;
			return SYNTAX_ERROR;
		}
		}
	*(adrs) = SYNTAX_ERROR;
	return SYNTAX_ERROR;
}

int command_syntax_check(int type,char* line){
	/*checks syntax of a command line. given command line's label has been removed. type is command type. 1 for 2 operand commands, 2 for 1 operand commands, 3 for 0 operand commands*/
	/*returns -1 for syntax error */

	if(type==1)
		return command_syntax_check_type1(line);

	if(type==2)
		return command_syntax_check_type2(line);

	if(type==3)
		return command_syntax_check_type3(line);
	return -1;
}


