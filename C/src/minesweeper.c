#include <assert.h>
#include <stdlib.h>
#include "../include/minesweeper.h"

//////////////////////////////////////
// CONSTANTS:
const char UNREVEALED = ' ' ;
const char REVEALED[9] = "012345678";
const char FLAG = 'F';
const char MINE = 'X';
//////////////////////////////////////

void generate_mines(struct board *b) {
	assert(b);

	for (int i = 0; i < b->num_mines; i++) {
		b->mines[i].x = 1 + rand() % b->width;
		b->mines[i].y = 1 + rand() % b->height;
		for (int j = i - 1; j >= 0; j--) {
			if (b->mines[i].x == b->mines[j].x && b->mines[i].y == b->mines[j].y) {
				i--;
				break;
			}
		}
	}
}

// within_bounds(b, x, y) checks if location (x, y) is within the boundaries of *b
// requires: *b is a valid board
// time: O(1)
static bool within_bounds(const struct board *b, int x, int y) {
	assert(b);
	return x > 0 && x <= b->width && y > 0 && y <= b->height;
}

// is_mine(b, x, y) returns true if there is a mine at location (x, y) on *b, 
// else returns false
// note: returns false for invalid locations 
// requires: *b is a valid board
// time: O(m) where m is the number of mines in *b
static bool is_mine(const struct board *b, int x, int y) {
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
	if (within_bounds(b, x, y)) {
		char *tile = &(b->grid)[(y - 1) * b->width + x - 1];
		if (*tile == UNREVEALED) {
			*tile = FLAG;
			b->mines_remaining--;
			return true;
		} else if (*tile == FLAG) {
			*tile = UNREVEALED;
			b->mines_remaining++;
			return true;
		}
	}

	return false;
}

// count_mines(b, x, y) returns the number of mines adjacent to (x, y) on *b
// requires: *b is a valid board
// time: O(m) where m is the number of mines in *b
static int count_mines(const struct board *b, int x, int y) {
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
	if (!within_bounds(b, x, y)) {
		return false;
	}
	char *tile = &(b->grid)[(y - 1) * b->width + x - 1];
	if (*tile != UNREVEALED || *tile == FLAG) {
		return false;
	} else if (is_mine(b, x, y)) {
		// a MINE is revealed - game is lost - show the remaining mines
		for (int j = 0; j < b->num_mines; j++) {
			int x1 = b->mines[j].x;
			int y1 = b->mines[j].y;
			char *tile = &(b->grid)[(y1 - 1) * b->width + x1 - 1];
			if (*tile != FLAG) {
				*tile = MINE;
			}
		}
		return true;
	}

	int z = count_mines(b, x, y);
	*tile = REVEALED[z];
	if (z == 0) {
		for (int x_offset = -1; x_offset <= 1; x_offset++) {
	  		for (int y_offset = -1; y_offset <= 1; y_offset++) {
				reveal(b, x + x_offset, y + y_offset);
	  		}
		}
	}

	b->revealed_count++;

	if (game_won(b)) {
		// flag the mines
		for (int j = 0; j < b->num_mines; j++) {
			int x1 = b->mines[j].x;
			int y1 = b->mines[j].y;
			b->grid[(y1 - 1) * b->width + x1 - 1] = FLAG;
		}
	}

	return true;
}

bool game_won(const struct board *b) {
	assert(b);
	return b->revealed_count == b->width * b->height - b->num_mines;
}

bool game_lost(const struct board *b) {
	assert(b);

	for (int i = 0; i < b->num_mines; i++) {
		int x = b->mines[i].x;
		int y = b->mines[i].y;
		char tile = b->grid[(y - 1) * b->width + x - 1];
		if (tile == MINE) {
			return true;
		}
	}

	return false;
}
