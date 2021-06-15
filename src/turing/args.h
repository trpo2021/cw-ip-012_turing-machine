#ifndef ARGS_H
#define ARGS_H

#define INPUT_DEFAULT "./turing.txt"
#define OUTPUT_DEFAULT NULL
#define STARTFILE_DEFAULT NULL
#define SILENTMODE_DEFAULT 0
#define STARTSTATE_DEFAULT "0"

#define MAX_FILENAME 32

typedef struct _params {
    char* input;
    char* output;
    char* startfile;
    char* startstate;
    short silent;
} Params;

void parse_args(Params*, int argc, char** argv);

char* get_input_filename();

void get_missing_params(Params*);

void print_params(Params);

char* get_startline(Params);

#endif
