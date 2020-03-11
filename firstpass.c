/*
 * firstpass.c
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */
#include "constants.h"
#include "firstpass.h"
#include "assembler.h"
#include "utils.h"
#include <errno.h>
#include <ctype.h>


int firstPass(char* fileName) {
	/* returns 0 on success, otherwise returns number of errors */
    int dataCount = 0;
    int instructionCount = 0;
    int errorCount = 0;
    int lineNumber = 0;
    int result;
    char* line;
    FILE* inputFile = fopen(fileName,"r");
    if (inputFile == NULL) {
        printf("err");
        return -1;
    }

    line = (char*)malloc(MAX_CMD);
    while (line, MAX_CMD, inputFile) {
        if ((result = processLine(++lineNumber, line, &dataCount, &instructionCount)) < 0) {
            ++errorCount;
        }
    }
    if (errorCount == 0) {
        update_data_symbol(instructionCount);
        free(line);
        return 0;
    }
    free(line);
    return errorCount;
}
