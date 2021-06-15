#include <libturing/checks.h>
#include <libturing/program.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

State create_state(char* name)
{
    State state = calloc(1, sizeof(State));
    check_allocated(state);

    state->name = calloc(MAX_LEN_STATENAME, sizeof(char));
    check_allocated(state->name);
    strcpy(state->name, name);

    Action* act_arr = calloc(INIT_LEN_ACTIONS, sizeof(Action));
    check_allocated(act_arr);

    state->actions = act_arr;
    state->len_actions = 0;
    state->max_actions = INIT_LEN_ACTIONS;

    return state;
}

Action
create_action(char symb_old, char symb_new, Motion motion, char* next_state)
{
    Action action = calloc(1, sizeof(Action));
    check_allocated(action);

    action->symb_old = symb_old;
    action->symb_new = symb_new;
    action->motion = motion;
    action->next_state = calloc(MAX_LEN_STATENAME, sizeof(char));
    check_allocated(action->next_state);

    strcpy(action->next_state, next_state);
    return action;
}

State add_action(Action action, State state)
{
    int i;
    for (i = 0; i < state->len_actions; i++) {
        if (action->symb_old == state->actions[i]->symb_old) {
#ifdef DEBUG
            fprintf(stderr,
                    "\nState `%s` with symb_old=`%c` is already exists\n",
                    state->name,
                    action->symb_old);
#endif
            return state;
        }
    }

    if (state->len_actions + 1 > state->max_actions) {
        state->max_actions *= 2;
        Action* ptemp
                = realloc(state->actions, sizeof(Action) * state->max_actions);
        check_allocated(ptemp);
        state->actions = ptemp;
    }
    state->actions[state->len_actions++] = action;

    return state;
}

Program create_program(void)
{
    Program prog;
    prog.names = calloc(INIT_LEN_PROGRAM, sizeof(char*));
    check_allocated(prog.names);
    prog.states = calloc(INIT_LEN_PROGRAM, sizeof(State));
    check_allocated(prog.states);
    prog.length = 0;
    prog.max_length = INIT_LEN_PROGRAM;
    return prog;
}

static int get_index_names(char* name, Program prog)
{
    int i;
    for (i = 0; i < prog.length; i++) {
        if (strcmp(name, prog.names[i]) == 0)
            return i;
    }
    return -1;
}

static int is_name_exists(char* name, Program prog)
{
    if (get_index_names(name, prog) == -1) {
        return 0;
    }
    return 1;
}

Program add_state(State st, Program prog)
{
    if (is_name_exists(st->name, prog) == 0) {
        // realloc
        if (prog.length >= prog.max_length - 1) {
            int new_length = prog.max_length * 2;
            char** names_temp = realloc(prog.names, new_length * sizeof(char*));
            State* states_temp
                    = realloc(prog.states, new_length * sizeof(State));
            check_allocated(names_temp);
            check_allocated(states_temp);
            prog.max_length = new_length;
            prog.names = names_temp;
            prog.states = states_temp;
        }
        // add state
        prog.names[prog.length] = calloc(MAX_LEN_STATENAME, sizeof(char));
        check_allocated(prog.names[prog.length]);
        strcpy(prog.names[prog.length], st->name);
        prog.states[prog.length] = st;
        prog.length++;
    } else {
        // check if State.actions[].old_symb in prog.states[idx_state].actions
        // if not exists => add action to State->actions
        int i, j, is_exists;
        int idx_state = get_index_names(st->name, prog);
        State s = prog.states[idx_state];
        for (i = 0; i < st->len_actions; i++) {
            is_exists = 0;
            for (j = 0; j < s->len_actions; j++) {
                if (st->actions[i]->symb_old == s->actions[j]->symb_old) {
                    is_exists = 1;
                    break;
                }
            }
            if (is_exists == 0) {
                add_action(st->actions[i], s);
            }
        }
    }
    return prog;
}

struct _released_list {
    Action pnode;
    struct _released_list* next;
};

static struct _released_list*
_add_released_list(Action paction, struct _released_list* list)
{
    struct _released_list* new_node = calloc(1, sizeof(struct _released_list));
    check_allocated(new_node);
    new_node->pnode = paction;
    new_node->next = list;
    return new_node;
}

static int _is_released(Action paction, struct _released_list* list)
{
    struct _released_list* prev;
    for (prev = list; prev->next; prev = prev->next)
        if (prev->pnode == paction)
            return 1;
    return 0;
}

void free_program(Program prog)
{
    int i, j;
    struct _released_list* list = calloc(1, sizeof(struct _released_list));
    check_allocated(list);
    list->next = NULL;
    Action paction;
    for (i = 0; i < prog.length; i++) {
        for (j = 0; j < prog.states[i]->len_actions; j++) {
            paction = prog.states[i]->actions[j];
            if (_is_released(paction, list) == 0) {
                free(paction);
                list = _add_released_list(paction, list);
            }
        }
        free(prog.names[i]);
    }
    struct _released_list* prev;
    while (list) {
        prev = list;
        list = list->next;
        free(prev);
    }
}

State get_state(char* name, Program prog)
{
    int i;
    for (i = 0; i < prog.length; i++) {
        if (strcmp(prog.names[i], name) == 0)
            return prog.states[i];
    }
    return NULL;
}

Action get_action(char* name, char symb_old, Program prog)
{
    State state = get_state(name, prog);
    if (state == NULL)
        return NULL;
    int i;
    for (i = 0; i < state->len_actions; i++) {
        if (state->actions[i]->symb_old == symb_old
            || state->actions[i]->symb_old == '*')
            return state->actions[i];
    }
    return NULL;
}

Program add_command(
        char* name,
        char symb_old,
        char symb_new,
        Motion motion,
        char* next,
        Program prog)
{
    State state = get_state(name, prog);
    if (state == NULL)
        state = create_state(name);

    prog = add_state(
            add_action(create_action(symb_old, symb_new, motion, next), state),
            prog);
    return prog;
}

void print_program(Program prog)
{
    int i, j;
    Action paction;

    for (i = 0; i < prog.length; i++) {
        printf("-----------------\n");
        printf("State: `%s`\n", prog.names[i]);
        printf("--\n");
        printf("Actions:\n");
        for (j = 0; j < prog.states[i]->len_actions; j++) {
            paction = prog.states[i]->actions[j];
            printf("\t`%c` : `%c` : `%d` : `%s` \n",
                   paction->symb_old,
                   paction->symb_new,
                   paction->motion,
                   paction->next_state);
        }
        printf("-----------------\n");
    }
}
