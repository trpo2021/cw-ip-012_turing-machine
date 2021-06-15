#include <libturing/checks.h>
#include <libturing/print_tape.h>
#include <libturing/program.h>
#include <libturing/tape.h>
#include <turing/args.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define start_state ("0")

void make_motion(Tape* tape, Node* cursor, Motion motion)
{
    switch (motion) {
    case MOTION_LEFT:
        if (*cursor == tape->head) {
            *tape = add_node_head('_', *tape);
        }
        if (*cursor == tape->tail) { // drop tail if it's empty
            if (is_node_empty(*cursor) == 1) {
                *cursor = (*cursor)->prev;
                *tape = rm_node_tail(*tape);
                break;
            }
        }
        *cursor = (*cursor)->prev;
        break;
    case MOTION_RIGHT:
        if (*cursor == tape->tail) {
            *tape = add_node_tail('_', *tape);
        }
        if (*cursor == tape->head) { // drop head if it's empty
            if (is_node_empty(*cursor) == 1) {
                *cursor = (*cursor)->next;
                *tape = rm_node_head(*tape);
                break;
            }
        }
        *cursor = (*cursor)->next;
        break;
    case MOTION_STAY:
    default:
        break;
    }
}

// run_action:
// modify tape, modify cursor
void run_action(Tape* tape, Action action, Node* cursor)
{
    char symb_new = action->symb_new;
    if (symb_new == '*') {
        if (action->symb_old == '*')
            symb_new = (*cursor)->symbol;
        else
            symb_new = action->symb_old;
    }
    check_symbol(symb_new);
    // change symbol on Tape
    (*cursor)->symbol = symb_new;
    make_motion(tape, cursor, action->motion);
}

void start_program(Program prog, Tape* tape, Node* cursor, Params rc)
{
    Action action;
    int is_halt = 0;
    char* statename = NULL;

    FILE* fout;

    if (rc.output) {
        fout = fopen(rc.output, "w");
    }

    check_statename_exists(prog, rc.startstate);
    action = get_action(rc.startstate, (*cursor)->symbol, prog);

    while (is_halt == 0) {
        run_action(tape, action, cursor);

        if (rc.silent == 0) {
            printf("Statename: %s\n", statename ? statename : start_state);

            printf("Action: %c %c %d %s\n",
                   action->symb_old,
                   action->symb_new,
                   action->motion,
                   action->next_state);
            print_tape(*tape, *cursor);
        }
        if (rc.output) {
            fprintf(fout,
                    "Statename: %s\n",
                    statename ? statename : start_state);
            fprintf(fout,
                    "Action: %c %c %d %s\n",
                    action->symb_old,
                    action->symb_new,
                    action->motion,
                    action->next_state);
            fprint_tape(fout, *tape, *cursor);
        }

        if (strncmp("halt", action->next_state, sizeof("halt")) == 0)
            is_halt = 1;

        statename = action->next_state;
        action = get_action(action->next_state, (*cursor)->symbol, prog);
    }
    if (rc.output)
        fclose(fout);
}
