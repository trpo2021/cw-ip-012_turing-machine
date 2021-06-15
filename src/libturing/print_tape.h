#ifndef PRINT_TAPE_H
#define PRINT_TAPE_H

#include <libturing/tape.h>

#include <stdio.h>

void print_tape(Tape, Node);
void fprint_tape(FILE*, Tape, Node);

#endif // PRINT_TAPE_H
