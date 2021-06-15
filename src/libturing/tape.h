#ifndef TAPE_H
#define TAPE_H

typedef struct node* Node;
struct node {
    char symbol;
    struct node* prev;
    struct node* next;
};

typedef struct tape Tape;
struct tape {
    Node head;
    Node tail;
    unsigned length;
};

Tape create_tape(void);

void free_tape(Tape);

int is_tape_empty(Tape);

Tape add_node_head(char, Tape);

Tape add_node_tail(char, Tape);

Tape rm_node_head(Tape);

Tape rm_node_tail(Tape);

int is_node_empty(Node);

// fill_tape:
// fill Tape object with symbols from string (line)
// replace ` ` with `_`
Tape fill_tape(char* line, Tape);

#endif // TAPE_H
