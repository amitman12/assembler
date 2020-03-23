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

int writeObjFile(struct assemblerContext *context) {
    FILE* output = fopenFileWithExt(context->fileName, "w", "ob");

    if (output==NULL) {
        fprintf(stderr, "could not open file \"%s.%s\". error: %s\n", context->fileName, "ob", strerror(errno));
        return -1;
    }
    /* write title line into output file */
    fprintf(output, "%d %d\n",context->instructionCount,context->dataCount);

    fwrite(context->objOut, strlen(context->objOut), 1, output);
    fclose(output);
    return 0;
}

int writeExtFile(struct assemblerContext *context) {
    FILE* output = fopenFileWithExt(context->fileName, "w", "ext");

    if (output==NULL) {
        fprintf(stderr, "could not open file \"%s.%s\". error: %s\n", context->fileName, "ext", strerror(errno));
        return -1;
    }
    fwrite(context->extOut, strlen(context->extOut), 1, output);
    fclose(output);
    return 0;
}


int secondPass(struct assemblerContext *context) {
    /* returns 0 on success, otherwise returns number of errors */
    int result;
    char *line;
    FILE* inputFile;
    inputFile = fopenFileWithExt(context->fileName,"r","as");
    if (inputFile == NULL) {
        fprintf(stderr, "could not open file \"%s\". error: %s\n", context->fileName, strerror(errno));
        return -1;
    }
    context->pass = SECOND_PASS;
    line = (char *) malloc(MAX_CMD);
    if (line == NULL) {
        fprintf(stderr, "could not allocate memory error: %s\n", strerror(errno));
        fclose(inputFile);
        free(line);
        return -1;
    }
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
        writeObjFile(context);
        if (context->extOut != NULL) {
            writeExtFile(context);
        }
        fclose(inputFile);
        return 0;
    }
    free(line);
    fclose(inputFile);
    return context->errorCount;
}


