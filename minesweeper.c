#include <stdio.h>
#include <assert.h>
#include "minesweeper.h"

//////////////////////////////////////
// CONSTANTS:
const char UNREVEALED = ' ' ;
const char REVEALED[9] = "012345678";
const char FLAG = 'F';
const char MINE = '*';
//////////////////////////////////////


// within_bounds(b, x, y) checks if location (x, y) is within the boundaries of *b
// requires: *b is a valid board
// time: O(1)
static bool within_bounds(struct board *b, int x, int y) {
	return x > 0 && x <= b->width && y > 0 && y <= b->height;
}

// is_mine(b, x, y) returns true if there is a mine at location (x, y) on *b, 
// else returns false
// note: returns false for invalid locations 
// requires: *b is a valid board
// time: O(n) where n is the number of mines in *b
static bool is_mine(struct board *b, int x, int y) {
	assert(b);
	if (within_bounds(b, x, y)) {
		for (int i = 0; i < b->num_mines; i++) {
			if (b->mines[i].x == x && b->mines[i].y == y) {
				return true;
			}
		}
	}
	return false;
}

bool flag(struct board *b, int x, int y) {
	assert(b);
	if (!within_bounds(b, x, y)) {
		return false;
	}
	char *tile = &(b->grid)[(y - 1) * b->width + x - 1];
	if (*tile == UNREVEALED) {
		*tile = FLAG;
		return true;
	} else if (*tile == FLAG) {
		*tile = UNREVEALED;
		return true;
	}
	return false;
}

// count_mines(b, x, y) returns the number of mines adjacent to (x, y) on *b
// requires: *b is a valid board
// time: O(n) where n is the number of mines in *b
static int count_mines(struct board *b, int x, int y) {
	assert(b);
	int count = 0;
	for (int x_offset = -1; x_offset <= 1; x_offset++) {
		for (int y_offset = -1; y_offset <= 1; y_offset++) {
			if (is_mine(b, x + x_offset, y + y_offset)) {
				count++;
	  		}
		}
	}
	return count;
}

bool reveal(struct board *b, int x, int y) {
	assert(b);
	if (!within_bounds(b, x, y)) {
		return false;
	}
	char *tile = &(b->grid)[(y - 1) * b->width + x - 1];
	if (*tile != UNREVEALED || *tile == FLAG) {
		return false;
	} else if (is_mine(b, x, y)) {
		*tile = MINE;
		return true;
	}
	int z = count_mines(b, x, y);
	*tile = REVEALED[z];
	if (z == 0) {
		for (int x_offset = -1; x_offset <= 1; x_offset++) {
	  		for (int y_offset = -1; y_offset <= 1; y_offset++) {
				if (within_bounds(b, x + x_offset, y + y_offset)) {
					reveal(b, x + x_offset, y + y_offset);
				}
	  		}
		}
	}
	return true;
}

bool game_won(const struct board *b) {
	assert(b);
	int num_revealed = 0;
	for (int x = 1; x <= b->width; x++) {
		for (int y = 1; y <= b->height; y++) {
			char tile = (b->grid)[(y - 1) * b->width + x - 1];
			if (tile == MINE) {
				return false;
			}
			if (tile != UNREVEALED && tile != FLAG) {
				num_revealed++;
			}
		}
	}
	if (num_revealed == (b->width) * (b->height) - (b->num_mines)) {
		return true;
	} else {
		return false;
	}
}

bool game_lost(const struct board *b) {
	assert(b);
	for (int i = 0; i < b->num_mines; i++) {
		int x = b->mines[i].x;
		int y = b->mines[i].y;
		char tile = (b->grid)[(y - 1) * b->width + x - 1];
		if (tile == MINE) {
	  		return true;
		}
	}
	return false;
}
