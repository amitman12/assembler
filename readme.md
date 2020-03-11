# comments

* reformat all fprintf to include file:line: error:...
* move directive processing to separate functions
* !!!!! remove the \t and " " from directive parsing
* create readString in utils
* use readNumber in ".data ", ".data\t" processing
* create tests


* rename file: symboletable to symbol_table
* rename struct: symboletable to symbol_table
* remove struct definition and variable from h file and move to .c
* usage documentation should be in the .h file near the declaration.
* implementation documentation should be near the implementation.
* rename edit_symbol to modify / update
* rename dealloc_table to dealloc_symbol_table
* use strcmp instead of eqls
* in symbol struct, better use enum for location
* in symbol struct, better use enum for type
* comments are always before
* in .c file include .h and avoid duplicating declarations
* global variables that should not be accessed by other modules should be static
* consider adding validations
* amit: edit_symbol(int address, location, type). update only uninitialized values
* all local functions should be declared as static (free_list)
* move eqls to utils or remove and use strcmp
* document pointers ownership in function usage
* in struct symbol change label to be array of X chars
* in constants.h define max label length
* move opcodes definition to constants.c. leave declaration as "extern" in .h
* 
* 



void parse_cmd1 () {

}

void parse_cmd2 () {
    
}


void first_pass() {
    char line[MAX_LEN];
    char label[MAX_LABEL_LEN];
    struct symbol* s;
    struct FILE* f;
    int eof = 0;
    f = fopen("a.as");
    if (f == NULL) {
        print not found;
        return;
    }

    while (!eof) {
        if (fgets(line, f) == NULL) {
            eof = 1;
        } else {
            (opcode, ....,) = parse_line(line, label)
            ok = 0;
            for (i = 0; opcodes[i] != NULL; ++i) {

                if (strcmp(opcodes[i], opcode) == 0) {
                    ok = 1;
                }
            }
            if (!OK) {
                print error;
                return;
            }

            opcodes[i].parse(line);
            if (i == 1 || i == 2 || i == 3) {

            }


            if (strcmp(opcode[i], "mov") == 0 || strcmp(opcodes[]))

            s = find_symbol(label)
            if (s == NULL) {
                create_symbol(label, ....);
            } else {
                maybe update

            }
        }
    }
}





-----------------------------------------------------------------------------------

next task
-------------

* create module main.c
* create module assembler.c, assembler.h
* create module first_pass.c, first_pass.h
* parse command line arguments
* call first pass for each command line argument



main.c
-------------

// argc - number of arguments (including the app name)
//        that means you start from 1
// argv - is the array of arguments
// argv[0] - app name
// argv[1] - arg1
int main(int argc, char*argv[]) {
    int i;
    if (argc < 2) {
        printf("expecting file names\n");
        printf("USAGE: assembler file...\n");
        exit(1);
    }
    for (i = 1; i < argc; ++i) {
        assembler(argv[i]);
    }
}


assembler.c
--------------------
int assembler(char* fileName) {
    firstPass(fileName);
}



first_pass.c
----------------------

int firstPass(char* fileName) {
    FILE* inputFile;

    inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "could not open file %s. error:%s\n", fileName, strerror(errno));
        return 1;
    }
    //CAUTION. must not return until end of function. must call fclose

    while (fgets(line, ..., inputFile)) {

    }

    fclose(inputFile);
}

// read on fopen, fgets, fclose
