#include <libturing/constants.h>
#include <libturing/program.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Errors {
    ERR_MEMORY_ALLOCATION = 2,
    ERR_WRONG_SYMBOL,
    ERR_STATENAME_LONG,
    ERR_STATENAME_CHAR,
    ERR_STATENAME_EXIST,
    ERR_ARGPARSE,
    ERR_FILE_OPEN,
    ERR_STARTLINE_LEN,
    ERR_STARTLINE_EMPTY,
};

void check_p_allocated(void* pname, int line)
{
    if (pname == NULL) {
        fprintf(stderr, "LINE %d: Cannot allocate memory!\n", line);
        exit(ERR_MEMORY_ALLOCATION);
    }
}

static int is_symb_in(char symbol, char* available)
{
    int i;
    for (i = 0; i < (int)strlen(available); i++) {
        if (symbol == available[i])
            return 1;
    }
    return 0;
}

static int is_symb_correct(char symbol)
{
    if (isalnum(symbol) || is_symb_in(symbol, AVAIL_SYMBOLS))
        return 1;
    return 0;
}

void check_symbol(char symbol)
{
    // check if symbol is correct
    if (is_symb_correct(symbol) == 0) {
        fprintf(stderr, "Wrong symbol: '%c'(%d)!\n", symbol, (int)symbol);
        exit(ERR_WRONG_SYMBOL);
    }
}

void print_debug_str(char* fstr, char* line)
{
    if (fstr == line) { // do nothing; compiler warn about unusable vars
    }
#ifdef DEBUG
    fprintf(stderr, fstr, line);
#endif
}

static void check_statename_len(const char* statename)
{
    if (strlen(statename) >= MAX_LEN_STATENAME) {
        fprintf(stderr,
                "Error! Too long statename, can't be more than %d! Statename: "
                "`%s` (%d symbols)\n",
                MAX_LEN_STATENAME - 1,
                statename,
                (int)strlen(statename));
        exit(ERR_STATENAME_LONG);
    }
}

static void check_statename_char(const char* statename)
{
    int i;
    for (i = 0; i < (int)strlen(statename); i++) {
        if (isalnum(statename[i]) == 0) {
            fprintf(stderr,
                    "Error! Can't use char `%c` in statename `%s`!\n",
                    statename[i],
                    statename);
            exit(ERR_STATENAME_CHAR);
        }
    }
}

void check_statename(const char* statename)
{
    check_statename_len(statename);
    check_statename_char(statename);
}

void print_help_message()
{
    fprintf(stderr, "%s\n", HELP_MESSAGE);
}

void raise_and_exit_argparse()
{
    print_help_message();
    exit(ERR_ARGPARSE);
}

void check_file_exists(char* filename)
{
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        fprintf(stderr, "Error! File `%s` doesn't exist!\n", filename);
        exit(ERR_FILE_OPEN);
    }
    fclose(fin);
}

static void check_startline_len(char* line)
{
    if (strlen(line) > MAX_STARTLINE) {
        fprintf(stderr,
                "Startline can't be longer than %d symbols! (You can change "
                "MAX_STARTLINE definition at ./src/libturing/constants.h)\n",
                MAX_STARTLINE);
        exit(ERR_STARTLINE_LEN);
    }
}

static void check_startline_empty(char* line)
{
    if (line == NULL || (line != NULL && line[0] == '\0')) {
        fprintf(stderr, "Error! Startline can't be an empty line!\n");
        exit(ERR_STARTLINE_EMPTY);
    };
}

void check_startline(char* line)
{
    check_startline_empty(line);
    check_startline_len(line);
}

static int is_statename_exists(Program prog, char* name)
{
    int i;
    for (i = 0; i < prog.length; i++)
        if (strcmp(name, prog.names[i]) == 0)
            return 1;
    return 0;
}

void check_statename_exists(Program prog, char* name)
{
    if (is_statename_exists(prog, name) == 0) {
        fprintf(stderr,
                "Error! Statename `%s` doesn't exists in that program!\n",
                name);
        exit(ERR_STATENAME_EXIST);
    }
}
