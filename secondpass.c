/*
 * secondpass.c
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */


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


int secondPass(struct assemblerContext *context) {
    /* returns 0 on success, otherwise returns number of errors */
    int result;
    char *line;
    FILE* inputFile;
    FILE* output;
    inputFile = fopenFileWithExt(context->fileName,"r","as");
    if (inputFile == NULL) {
        fprintf(stderr, "could not open file \"%s\". error: %s\n", context->fileName, strerror(errno));
        return -1;
    }
    output = fopenFileWithExt(context->fileName,"w","ob");
    if (output==NULL) {
        fprintf(stderr, "could not open file \"%s\". error: %s\n", context->fileName, strerror(errno));
        fclose(inputFile);
        return -1;
    }
    context->objFile = output;
    context->pass = SECOND_PASS;
    line = (char *) malloc(MAX_CMD);
    if (line == NULL) {
        fprintf(stderr, "could not allocate memory error: %s\n", strerror(errno));
        fclose(inputFile);
        free(line);
        return -1;
    }
    /* write title line into output file */
	fprintf(output, "%d %d\n",context->instructionCount,context->dataCount);
	context->instructionCount = 0;
    while (fgets(line, MAX_CMD, inputFile)) {

        chomp(line);
        ++context->lineNumber;
        if ((result = processLine(context, line)) < 0) {
            ++context->errorCount;
        }
    }
    if (context->errorCount == 0) {
        free(line);
        outDirective(context);
        fclose(inputFile);
        return 0;
    }
    free(line);
    fclose(inputFile);
    return context->errorCount;
}


