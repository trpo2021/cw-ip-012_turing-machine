#ifndef PROGRAM_H
#define PROGRAM_H

#include <libturing/constants.h>

typedef enum Motion {
    MOTION_LEFT,
    MOTION_RIGHT,
    MOTION_STAY,
} Motion;

typedef struct action* Action;
struct action {
    char symb_old;
    char symb_new;
    Motion motion;
    char* next_state;
};

typedef struct state* State;
struct state {
    char* name;
    Action* actions; // pointer to array of actions
    int len_actions;
    int max_actions; // allocated memory for array actions
};

typedef struct program Program;
struct program {
    char** names;
    State* states;
    int length;     // len of arrays `names` and `states`
    int max_length; // allocated memory for `names` and `states`
};

Program create_program(void);

void free_program(Program);

// get_state:
// return State or NULL
State get_state(char* name, Program);

// get_action:
// return Action or NULL
Action get_action(char* name, char symb, Program);

Program
add_command(char* name, char c_old, char c_new, Motion, char* next, Program);

void print_program(Program);

#endif // PROGRAM_H
