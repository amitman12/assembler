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
#include "secondpass.h"
#include <errno.h>
#include <ctype.h>


int firstPass(struct assemblerContext* context) {
	/* returns 0 on success, otherwise returns number of errors */
    int result;
    char* line;
    FILE* inputFile = fopenFileWithExt(context->fileName,"r","as");
    if (inputFile == NULL) {
        fprintf(stderr,"could not open file \"%s\". error: %s\n", context->fileName, strerror(errno));
        return -1;
    }

    line = (char*)malloc(MAX_CMD);
    if(line==NULL){
        fprintf(stderr,"could not allocate memory error: %s\n", strerror(errno));
    }
    while (fgets(line, MAX_CMD, inputFile)) {
    	chomp(line);
        ++context->lineNumber;
        if ((result = processLine(context, line, FIRST_PASS)) < 0) {
            ++context->errorCount;
        }
    }
    if (context->errorCount == 0) {
        update_data_symbol(&context->table, context->instructionCount);
        free(line);
        fclose(inputFile);
        /* call second pass if no errors */
        secondPass(context);
        return 0;
    }
    free(line);
    fclose(inputFile);
    return context->errorCount;
}
