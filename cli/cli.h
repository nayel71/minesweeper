#ifndef CLI_H
#define CLI_H

#include "../src/minesweeper.h"

// print_board(b) displays the minesweeper board *b in a nice format
// requires: *b is a valid board
void print_board(const struct board *b);

// prints a list of commands
void print_commands(void);

// starts a command-line minesweeper game
// effects: lots of i/o
int play_cli(struct board *b);

#endif /* CLI_H */
