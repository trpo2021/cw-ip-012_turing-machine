#include <libturing/checks.h>
#include <libturing/constants.h>
#include <libturing/tape.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node create_node(char symbol)
{
    Node new_node = calloc(1, sizeof(struct node));
    check_allocated(new_node);
    check_symbol(symbol);
    new_node->symbol = symbol;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

int is_tape_empty(Tape tape)
{
    if (tape.head == tape.tail && tape.head->symbol == SYMB_NULL)
        return 1;
    return 0;
}

Tape create_tape()
{
    Tape tape;
    tape.head = create_node(SYMB_NULL);
    tape.tail = tape.head;
    tape.length = 0;
    return tape;
}

Tape add_node_head(char symbol, Tape tape)
{
    if (is_tape_empty(tape)) {
        tape.head->symbol = symbol;
        tape.length++;
        return tape;
    }
    Node new_head = create_node(symbol);
    tape.head->prev = new_head;
    new_head->next = tape.head;
    tape.head = new_head;
    tape.length++;
    return tape;
}

Tape add_node_tail(char symbol, Tape tape)
{
    if (is_tape_empty(tape)) {
        tape.head->symbol = symbol;
        tape.length++;
        return tape;
    }
    Node new_tail = create_node(symbol);
    tape.tail->next = new_tail;
    new_tail->prev = tape.tail;
    tape.tail = new_tail;
    tape.length++;
    return tape;
}

Tape rm_node_head(Tape tape)
{
    if (is_tape_empty(tape)) {
        return tape;
    }
    if (tape.head == tape.tail) {
        tape.head->symbol = SYMB_NULL;
        tape.length--;
        return tape;
    }

    Node prev_head = tape.head;
    tape.head = tape.head->next;
    tape.head->prev = NULL;
    tape.length--;
    free(prev_head);
    return tape;
}

Tape rm_node_tail(Tape tape)
{
    if (is_tape_empty(tape)) {
        return tape;
    }
    if (tape.head == tape.tail) {
        tape.head->symbol = SYMB_NULL;
        tape.length--;
        return tape;
    }

    Node prev_tail = tape.tail;
    tape.tail = tape.tail->prev;
    tape.tail->next = NULL;
    tape.length--;
    free(prev_tail);
    return tape;
}

void free_tape(Tape tape)
{
    Node head = tape.head;
    for (Node prev = head; head;) {
        prev = head;
        head = head->next;
        free(prev);
    }
}

int is_node_empty(Node node)
{
    if (node->symbol == SYMB_NULL)
        return 1;
    return 0;
}

Tape fill_tape(char* line, Tape tape)
{
    int i, c;
    int length = (int)strlen(line);
    for (i = 0; i < length; i++) {
        c = line[i];
        c = (c == ' ') ? '_' : c;
        check_symbol(c);
        tape = add_node_tail(c, tape);
    }
    return tape;
}
