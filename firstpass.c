/*
 * firstpass.c
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */
#include "constants.h"
#include <errno.h>
int firstPass(char* fileName) {
    FILE* inputFile;
    char* line;
    inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "could not open file %s. error:%s\n", fileName, strerror(errno));
        return 1;
    }
    /*CAUTION. must not return until end of function. must call fclose*/

	line = malloc(MAX_CHARACTERS_IN_COMMAND);
    while (fgets(line, MAX_CHARACTERS_IN_COMMAND, inputFile)) {

    }

    fclose(inputFile);
}

