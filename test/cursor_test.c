#include <libturing/cursor.h>
#include <libturing/tape.h>

#include <ctest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CTEST(suite_cursor, make_motion)
{
    Tape tape = create_tape();
    tape = fill_tape("abcd", tape);

    Node cursor = tape.head;
    Motion motion = MOTION_LEFT;

    make_motion(&tape, &cursor, motion);

    ASSERT_NOT_NULL(cursor);
    ASSERT_EQUAL(cursor, tape.head);
    ASSERT_EQUAL('_', tape.head->symbol);
    ASSERT_EQUAL('a', tape.head->next->symbol);

    motion = MOTION_RIGHT;
    make_motion(&tape, &cursor, motion);

    ASSERT_NOT_NULL(cursor);
    ASSERT_EQUAL(cursor, tape.head);
    ASSERT_EQUAL('a', tape.head->symbol);
    ASSERT_EQUAL('b', tape.head->next->symbol);
    ASSERT_NULL(cursor->prev);
}

CTEST(suite_cursor, run_action)
{
    Tape tape = create_tape();
    tape = fill_tape("abcd", tape);

    Node cursor = tape.head;

    Action action = calloc(1, sizeof(Action));
    action->symb_old = 'a';
    action->symb_new = 'b';
    action->motion = MOTION_LEFT;
    action->next_state = "q2";

    run_action(&tape, action, &cursor);

    ASSERT_NOT_NULL(cursor);
    ASSERT_EQUAL(cursor, tape.head);
    ASSERT_EQUAL('_', tape.head->symbol);
    ASSERT_EQUAL('b', tape.head->next->symbol);
}
