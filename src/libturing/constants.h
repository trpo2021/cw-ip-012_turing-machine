#ifndef CONSTANTS_H
#define CONSTANTS_H

// max length of statename
#define MAX_LEN_STATENAME 64

// initial length of arrays: State.actions and Program.states
// used in: src/libturing/program.c
#define INIT_LEN_ACTIONS 1
#define INIT_LEN_PROGRAM 4

// available symbols on tape
// you can't use spaces ` ` (use `_` instead) and `;`
// used in: src/libturing/checks.c
#define AVAIL_SYMBOLS "*_+[]<>:!#-,.()?"

// NULL-symbol
// used for pseudo-element (when tape is empty)
#define SYMB_NULL '_'

#define HELP_MESSAGE                                                  \
    "Usage: turing [-f INPUT_FILE] [-o OUTPUT_FILE] [-s "             \
    "STARTLINE_FILE] [-t INITIAL_STATENAME] [--silent]\n\n"           \
    "\t-f <filename> \tInput filename with code for turing machine\n" \
    "\t-o <filename> \tOutput filename\n"                             \
    "\t-s <filename> \tFilename with start line for tape\n"           \
    "\t-t <statename> \tInitial statename\n"                          \
    "\t--silent\tEnable silent mode"

#define MAX_STARTLINE 512

#endif // CONSTANTS_H
