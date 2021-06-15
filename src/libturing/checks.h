#ifndef CHECKS_H
#define CHECKS_H

#include <libturing/program.h>

#define check_allocated(p) check_p_allocated((void*)p, __LINE__)

void check_p_allocated(void*, int line); // ERR_MEMORY_ALLOCATION

// check if symbol correct
void check_symbol(char); // ERR_WRONG_SYMBOL

void print_debug_str(char* fstr, char* line); // warning, doesn't raise error

void check_statename(char*); // ERR_STATEMENT_LONG or ERR_STATEMENT_CHAR

void print_help_message();

void raise_and_exit_argparse(); // ERR_ARGPARSE

void check_file_exists(char*); // ERR_FILE_OPEN

void check_startline(char*); // ERR_STARTLINE_LEN or ERR_STARTLINE_EMPTY

void check_statename_exists(Program, char*); // ERR_STATENAME_EXIST

#endif // CHECKS_H
