#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdbool.h>

// A module with rules for the minesweeper game (based on CS 136 assignment)

struct tile {
	int x;
	int y;
};

extern const char UNREVEALED;
extern const char REVEALED[];
extern const char FLAG;
extern const char MINE;

// in a valid board:
// * width > 0, height > 0
// * (width * height) is the length of grid (it is a char array, not a string)
// * coordinate (x, y) is stored in grid[(y - 1) * width + x - 1]
//	the lop-left coordinate is at (1, 1), bottom-right is (width, height)
// * each element of grid is one of:
//	UNREVEALED, REVEALED[0..8], FLAG or MINE
// * num_mines is the length of mines,
//	each mine is at a valid and unique coordinate

struct board {
	int width;
	int height;
	char *grid;
	int num_mines;
	struct tile *mines;
};


// generate_mines(b) randomly generates the mines on the minesweeper board *b
// requires: *b is a valid board
// effects: modifies b->mines
// time: ???

void generate_mines(struct board *b);


// flag(b, x, y) places a FLAG on an UNREVEALED tile (x, y) or makes a
//	previously flagged tile UNREVEALED.
//	Returns true if successful (a flag was placed or removed)
//	and false if the tile (x, y) was already revealed.
// note: returns false for invalid locations.
// requires: *b is a valid board
// effects: tile (x, y) of board b may change
// time: O(1) 

bool flag(struct board *b, int x, int y);


// reveal(b, x, y) reveals (x, y) and replaces it with either a MINE
//	or REVEALED[z] where z is the number of mines adjacent to (x, y).
//	Returns true if successful [(x, y) was UNREVEALED and now revealed]
//	and false if the tile (x, y) was already REVEALED, a MINE or a FLAG.
//	If z is 0, it also reveals all unrevealed adjacent tiles (neighbours),
//	and (recursively) continues to do so until no more 0's are revealed.
// note: returns false for invalid locations.
// requires: *b is a valid board
// effects: some tiles may be revealed
// time: O(nm) where n is the number of tiles and m is the number of mines in b

bool reveal(struct board *b, int x, int y);


// game_won(b) returns true if all safe (mine-free) tiles have been revealed,
//	and no mines have been revealed.
//	Returns false otherwise.
// requires: *b is a valid board
// effects: FLAGs all the mines
// time: O(1)

bool game_won(const struct board *b);


// game_lost(b) returns true if there are any MINEs on board b.
//	Returns false otherwise.
// requires: *b is a valid board
// effects: displays all the mines
// time: O(m) where m is the number of mines in b

bool game_lost(const struct board *b);

#endif /* MINESWEEPER_H */
