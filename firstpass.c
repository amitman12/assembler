/*
 * firstpass.c
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */
#include "constants.h"
#include "firstpass.h"
#include "assembler.h"
#include "symboltable.h"
#include "utils.h"
#include <errno.h>
#include <ctype.h>


int firstPass(struct assemblerContext* context) {
	/* returns 0 on success, otherwise returns number of errors */
    int result;
    char* line;
    FILE* inputFile = fopen(context->fileName,"r");
    if (inputFile == NULL) {
        fprintf(stderr,"could not open file \"%s\". error: %s\n", context->fileName, strerror(errno));
        return -1;
    }

    line = (char*)malloc(MAX_CMD);
    while (fgets(line, MAX_CMD, inputFile)) {
    	chomp(line);
        ++context->lineNumber;
        if ((result = processLine(context, line)) < 0) {
            ++context->errorCount;
        }
    }
    if (context->errorCount == 0) {
        update_data_symbol(&context->table, context->instructionCount);
        free(line);
        return 0;
    }
    free(line);
    return context->errorCount;
}
