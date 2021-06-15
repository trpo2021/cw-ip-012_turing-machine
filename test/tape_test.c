#include <libturing/constants.h>
#include <libturing/tape.h>

#include <libturing/tape.c>

#include <ctest.h>
#include <stdlib.h>

#define CTEST_SEGFAULT

CTEST(suite_tape, create_tape)
{
    Tape tape = create_tape();
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.tail->next);
    ASSERT_EQUAL(SYMB_NULL, tape.head->symbol);
    ASSERT_EQUAL(SYMB_NULL, tape.tail->symbol);
    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.tail->next);
    ASSERT_NULL(tape.tail->prev);

    free_tape(tape);
}

CTEST(suite_tape, add_tape_head)
{
    char symbol = 'a';
    Tape tape = create_tape();
    tape = add_node_head(symbol, tape);

    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_EQUAL(symbol, tape.head->symbol);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.head->next);

    char symbol2 = 'b';
    tape = add_node_head(symbol2, tape);

    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.tail->next);
    ASSERT_NOT_NULL(tape.head->next);
    ASSERT_NOT_NULL(tape.tail->prev);
    ASSERT_NULL(tape.head->next->next);
    ASSERT_NULL(tape.tail->prev->prev);
    ASSERT_EQUAL(symbol2, tape.head->symbol);
    ASSERT_EQUAL(symbol, tape.head->next->symbol);
    ASSERT_EQUAL(symbol, tape.tail->symbol);
    ASSERT_EQUAL(symbol2, tape.tail->prev->symbol);

    free_tape(tape);
}

CTEST(suite_tape, add_node_tail)
{
    Tape tape = create_tape();

    tape = add_node_tail('a', tape);

    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.head->next);
    ASSERT_EQUAL('a', tape.head->symbol);

    tape = add_node_tail('b', tape);

    ASSERT_FALSE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.tail->next);
    ASSERT_NOT_NULL(tape.head->next);
    ASSERT_NOT_NULL(tape.tail->prev);
    ASSERT_EQUAL('a', tape.head->symbol);
    ASSERT_EQUAL('b', tape.tail->symbol);
    ASSERT_EQUAL('a', tape.tail->prev->symbol);
    ASSERT_EQUAL('b', tape.head->next->symbol);

    tape = add_node_tail('c', tape);

    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.tail->next);
    ASSERT_NOT_NULL(tape.head->next);
    ASSERT_EQUAL('a', tape.head->symbol);
    ASSERT_EQUAL('b', tape.head->next->symbol);
    ASSERT_EQUAL('c', tape.head->next->next->symbol);
    ASSERT_EQUAL('c', tape.tail->symbol);
    ASSERT_EQUAL('b', tape.tail->prev->symbol);
    ASSERT_EQUAL('a', tape.tail->prev->prev->symbol);

    free_tape(tape);
}

CTEST(suite_tape, rm_node_head)
{
    Tape tape = create_tape();

    tape = rm_node_head(tape);
    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->next);
    ASSERT_NULL(tape.head->prev);
    ASSERT_EQUAL(SYMB_NULL, tape.head->symbol);

    tape = add_node_head('a', tape);
    tape = rm_node_head(tape);

    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.head->next);
    ASSERT_EQUAL(SYMB_NULL, tape.head->symbol);

    free_tape(tape);

    tape = create_tape();

    tape = add_node_head('a', tape);
    tape = add_node_head('b', tape);
    tape = rm_node_head(tape);

    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.head->next);
    ASSERT_EQUAL('a', tape.head->symbol);

    tape = add_node_head('b', tape);
    tape = add_node_head('c', tape);
    tape = rm_node_head(tape);

    ASSERT_FALSE(tape.head == tape.tail);
    ASSERT_TRUE(tape.head == tape.tail->prev);
    ASSERT_TRUE(tape.head->next == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.tail->next);
    ASSERT_NOT_NULL(tape.head->next);
    ASSERT_NOT_NULL(tape.tail->prev);
    ASSERT_EQUAL('b', tape.head->symbol);
    ASSERT_EQUAL('a', tape.head->next->symbol);

    free_tape(tape);
}

CTEST(suite_tape, rm_node_tail)
{
    Tape tape = create_tape();

    tape = rm_node_tail(tape);
    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->next);
    ASSERT_NULL(tape.head->prev);
    ASSERT_EQUAL(SYMB_NULL, tape.head->symbol);

    tape = add_node_head('a', tape);
    tape = rm_node_tail(tape);

    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.head->next);
    ASSERT_EQUAL(SYMB_NULL, tape.head->symbol);

    free_tape(tape);

    tape = create_tape();

    tape = add_node_head('a', tape);
    tape = add_node_head('b', tape);
    tape = rm_node_tail(tape);

    ASSERT_TRUE(tape.head == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.head->next);
    ASSERT_EQUAL('b', tape.head->symbol);

    tape = add_node_head('a', tape);
    tape = add_node_head('c', tape);
    tape = rm_node_tail(tape);

    ASSERT_FALSE(tape.head == tape.tail);
    ASSERT_TRUE(tape.head == tape.tail->prev);
    ASSERT_TRUE(tape.head->next == tape.tail);
    ASSERT_NULL(tape.head->prev);
    ASSERT_NULL(tape.tail->next);
    ASSERT_NOT_NULL(tape.head->next);
    ASSERT_NOT_NULL(tape.tail->prev);
    ASSERT_EQUAL('c', tape.head->symbol);
    ASSERT_EQUAL('a', tape.head->next->symbol);

    free_tape(tape);
}

CTEST(suite_tape, is_tape_empty)
{
    int result_exp, result;

    Tape tape = create_tape();

    result_exp = 1;
    result = is_tape_empty(tape);
    ASSERT_EQUAL(result_exp, result);

    tape = add_node_head('a', tape);
    result_exp = 0;
    result = is_tape_empty(tape);
    ASSERT_EQUAL(result_exp, result);

    tape = rm_node_head(tape);
    result_exp = 1;
    result = is_tape_empty(tape);
    ASSERT_EQUAL(result_exp, result);

    free_tape(tape);
}

CTEST(suite_tape, create_node)
{
    Node n1 = create_node(SYMB_NULL);
    ASSERT_EQUAL(SYMB_NULL, n1->symbol);
    ASSERT_NULL(n1->prev);
    ASSERT_NULL(n1->next);
    free(n1);

    n1 = create_node('a');
    ASSERT_EQUAL('a', n1->symbol);
    ASSERT_NULL(n1->prev);
    ASSERT_NULL(n1->next);
    free(n1);
}

CTEST(suite_tape, is_node_empty)
{
    int res;
    Node n1;

    n1 = create_node(SYMB_NULL);
    res = is_node_empty(n1);
    ASSERT_EQUAL(1, res);
    free(n1);

    n1 = create_node('a');
    res = is_node_empty(n1);
    ASSERT_EQUAL(0, res);
    free(n1);
}

CTEST(suite_tape, check_length_correct)
{
    Tape tape = create_tape();

    ASSERT_EQUAL(0, tape.length);

    tape = rm_node_head(tape);

    ASSERT_EQUAL(0, tape.length);

    tape = add_node_head('a', tape);

    ASSERT_EQUAL(1, tape.length);

    tape = add_node_head('b', tape);

    ASSERT_EQUAL(2, tape.length);

    tape = rm_node_tail(tape);

    ASSERT_EQUAL(1, tape.length);

    tape = rm_node_head(tape);

    ASSERT_EQUAL(0, tape.length);

    tape = rm_node_tail(tape);

    ASSERT_EQUAL(0, tape.length);

    free_tape(tape);
}
