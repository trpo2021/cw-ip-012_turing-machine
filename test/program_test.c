#include <libturing/program.h>

#include <libturing/program.c>

#include <ctest.h>
#include <stdlib.h>
#include <string.h>

CTEST(suite_program, create_state)
{
    State state_exp = calloc(1, sizeof(State));
    State state_res = create_state("q1");

    state_exp->name = calloc(MAX_LEN_STATENAME, sizeof(char));
    strcpy(state_exp->name, "q1");
    state_exp->len_actions = 0;
    state_exp->max_actions = INIT_LEN_ACTIONS;

    ASSERT_STR(state_exp->name, state_res->name);
    ASSERT_EQUAL(state_exp->len_actions, state_res->len_actions);
    ASSERT_EQUAL(state_exp->max_actions, state_res->max_actions);
    ASSERT_NOT_NULL(state_res->actions);

    free(state_res->actions);
    free(state_res);
}

CTEST(suite_program, create_action)
{
    Action act_res;

    char symb_old = 'a';
    char symb_new = 'b';
    Motion motion = MOTION_LEFT;
    char* next_state = "q2";

    act_res = create_action(symb_old, symb_new, motion, next_state);

    ASSERT_STR(next_state, act_res->next_state);
    ASSERT_EQUAL(symb_old, act_res->symb_old);
    ASSERT_EQUAL(symb_new, act_res->symb_new);
    ASSERT_EQUAL(motion, act_res->motion);
}

CTEST(suite_program, add_action)
{
    State st = create_state("q1");

    st = add_action(create_action('a', 'b', MOTION_STAY, "q2"), st);
    st = add_action(create_action('b', 'c', MOTION_LEFT, "q1"), st);

    ASSERT_STR("q1", st->name);
    ASSERT_EQUAL(2, st->len_actions);

    ASSERT_EQUAL('a', st->actions[0]->symb_old);
    ASSERT_EQUAL('b', st->actions[1]->symb_old);
    ASSERT_EQUAL('b', st->actions[0]->symb_new);
    ASSERT_EQUAL('c', st->actions[1]->symb_new);
    ASSERT_EQUAL(MOTION_STAY, st->actions[0]->motion);
    ASSERT_EQUAL(MOTION_LEFT, st->actions[1]->motion);
    ASSERT_STR("q2", st->actions[0]->next_state);
    ASSERT_STR("q1", st->actions[1]->next_state);

    free(st->actions[0]);
    free(st->actions[1]);
    free(st);
}

CTEST(suite_program, create_program)
{
    Program prog = create_program();

    ASSERT_NOT_NULL(prog.names);
    ASSERT_NOT_NULL(prog.states);
    ASSERT_EQUAL(0, prog.length);
    ASSERT_EQUAL(INIT_LEN_PROGRAM, prog.max_length);

    free_program(prog);
}

CTEST(suite_program, add_state)
{
    Program prog = create_program();

    State s1 = create_state("q1");
    Action a1 = create_action('a', '*', MOTION_LEFT, "q2");
    s1 = add_action(a1, s1);

    prog = add_state(s1, prog);

    ASSERT_NOT_NULL(prog.names);
    ASSERT_NOT_NULL(prog.states);
    ASSERT_STR("q1", prog.names[0]);
    ASSERT_EQUAL('a', prog.states[0]->actions[0]->symb_old);
    ASSERT_EQUAL(1, prog.states[0]->len_actions);
    ASSERT_EQUAL(1, prog.length);

    prog = add_state(s1, prog);

    ASSERT_EQUAL('a', prog.states[0]->actions[0]->symb_old);
    ASSERT_EQUAL(1, prog.states[0]->len_actions);
    ASSERT_EQUAL(1, prog.length);

    Action a2 = create_action('b', '*', MOTION_LEFT, "q2");
    s1 = add_action(a1, s1);
    s1 = add_action(a2, s1);
    prog = add_state(s1, prog);
    ASSERT_EQUAL('a', prog.states[0]->actions[0]->symb_old);
    ASSERT_EQUAL(2, prog.states[0]->len_actions);
    ASSERT_EQUAL(1, prog.length);

    Action a3 = create_action('c', 'a', MOTION_RIGHT, "q1");
    s1 = add_action(a1, s1);
    s1 = add_action(a2, s1);
    s1 = add_action(a3, s1);
    prog = add_state(s1, prog);
    ASSERT_EQUAL('a', prog.states[0]->actions[0]->symb_old);
    ASSERT_EQUAL('b', prog.states[0]->actions[1]->symb_old);
    ASSERT_EQUAL('c', prog.states[0]->actions[2]->symb_old);
    ASSERT_EQUAL(3, prog.states[0]->len_actions);
    ASSERT_EQUAL(1, prog.length);

    State s2 = create_state("q2");
    s2 = add_action(a1, s2);
    s2 = add_action(a2, s2);
    prog = add_state(s2, prog);
    ASSERT_EQUAL('a', prog.states[1]->actions[0]->symb_old);
    ASSERT_EQUAL('b', prog.states[1]->actions[1]->symb_old);
    ASSERT_EQUAL(2, prog.states[1]->len_actions);
    ASSERT_EQUAL(2, prog.length);

    prog.states[1] = add_action(a3, prog.states[1]);
    ASSERT_EQUAL('c', prog.states[1]->actions[2]->symb_old);
    ASSERT_EQUAL(3, prog.states[1]->len_actions);
    ASSERT_EQUAL(2, prog.length);

    free_program(prog);
}

CTEST(suite_program, get_state)
{
    Program prog = create_program();

    State result = get_state("q1", prog);

    ASSERT_NULL(result);

    prog = add_state(
            add_action(
                    create_action('a', 'b', MOTION_LEFT, "q2"),
                    create_state("q1")),
            prog);
    prog = add_state(
            add_action(
                    create_action('b', 'c', MOTION_LEFT, "q2"),
                    create_state("q1")),
            prog);
    prog = add_state(
            add_action(
                    create_action('a', 'c', MOTION_LEFT, "q1"),
                    create_state("q2")),
            prog);

    result = get_state("q1", prog);

    ASSERT_NOT_NULL(result);
    ASSERT_NOT_NULL(result->actions);
    ASSERT_EQUAL('a', result->actions[0]->symb_old);
    ASSERT_EQUAL('b', result->actions[1]->symb_old);

    result = get_state("q2", prog);
    ASSERT_NOT_NULL(result);
    ASSERT_NOT_NULL(result->actions);
    ASSERT_EQUAL('a', result->actions[0]->symb_old);

    result = get_state("q3", prog);
    ASSERT_NULL(result);

    free_program(prog);
}

CTEST(suite_program, get_action)
{
    Program prog = create_program();

    Action result = get_action("q1", 'a', prog);

    ASSERT_NULL(result);

    prog = add_state(
            add_action(
                    create_action('a', 'b', MOTION_LEFT, "q2"),
                    create_state("q1")),
            prog);
    prog = add_state(
            add_action(
                    create_action('b', 'c', MOTION_LEFT, "q2"),
                    create_state("q1")),
            prog);
    prog = add_state(
            add_action(
                    create_action('a', 'c', MOTION_LEFT, "q1"),
                    create_state("q2")),
            prog);

    result = get_action("q1", 'a', prog);
    ASSERT_NOT_NULL(result);
    ASSERT_EQUAL('a', result->symb_old);
    ASSERT_EQUAL('b', result->symb_new);

    result = get_action("q1", 'b', prog);
    ASSERT_NOT_NULL(result);
    ASSERT_EQUAL('b', result->symb_old);
    ASSERT_EQUAL('c', result->symb_new);

    result = get_action("q2", 'b', prog);
    ASSERT_NULL(result);

    prog = add_state(
            add_action(
                    create_action('*', '1', MOTION_LEFT, "q1"),
                    create_state("q2")),
            prog);

    result = get_action("q2", '4', prog);
    ASSERT_NOT_NULL(result);
    ASSERT_EQUAL('*', result->symb_old);
    ASSERT_EQUAL('1', result->symb_new);

    free_program(prog);
}

CTEST(suite_program, add_command)
{
    Program prog = create_program();

    prog = add_command("q1", 'a', 'b', MOTION_LEFT, "q2", prog);

    prog = add_command("q1", 'b', 'c', MOTION_RIGHT, "q2", prog);

    prog = add_command("q2", 'a', 'c', MOTION_STAY, "q1", prog);

    State q1 = get_state("q1", prog);

    ASSERT_NOT_NULL(q1);
    ASSERT_NOT_NULL(q1->actions);
    ASSERT_EQUAL('a', q1->actions[0]->symb_old);
    ASSERT_EQUAL('b', q1->actions[1]->symb_old);

    State q2 = get_state("q2", prog);
    ASSERT_NOT_NULL(q2);
    ASSERT_NOT_NULL(q2->actions);
    ASSERT_EQUAL('a', q2->actions[0]->symb_old);

    State q3 = get_state("q3", prog);
    ASSERT_NULL(q3);

    free_program(prog);
}
