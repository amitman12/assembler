/*
 * main.c
 *
 *  Created on: Jan 4, 2020
 *      Author: user
 */
#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char*argv[]) {
    int i;
    if (argc < 2) {
        fprintf(stderr,"expecting file names\n");
        fprintf(stderr,"USAGE: assembler file...\n");
        exit(1);
    }
    for (i = 1; i < argc; ++i) {
        assembler(argv[i]);
    }
}

