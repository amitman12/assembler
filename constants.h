/*
 * constants.h
 *
 *  Created on: Dec 27, 2019
 *      Author: user
 */

#ifndef CONSTANTS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONSTANTS_H_
#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define INDIRECT_REGISTER_ADDRESSING 2
#define DIRECT_REGISTER_ADDRESSING 3
#define MAX_LABEL_LENGTH 50
#define MAX_CHARACTERS_IN_COMMAND 1000

extern char* opcodes[];
enum location {Code = 1, Data = 0, Unknown_Location = -1};
enum type {External = 1, Entry = 0, Unknown_Type = -1};


#endif /* CONSTANTS_H_ */
