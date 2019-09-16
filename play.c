#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "minesweeper.h"

// generate_mines(b) randomly generates the mines on the minesweeper board *b
// requires: *b is a valid board
static void generate_mines(struct board *b) {
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

// print_board(b) displays the minesweeper board *b in a nice format
// requires: *b is a valid board
static void print_board(const struct board *b) {
	assert(b);
	int height = b->height + 1;
	int width = 2 * b->width + 4;
	for (int i = 0; i < width; i++) {
		if (i == 2) {
			printf("|");
		} else {
			if (i % 2 == 0 && i > 1) {
				printf("%c", 'a' - 1 + (i - 2) / 2);
			} else {
				printf("_");
			}
		}
	}
	printf("\n");
	for (int j = 1; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int y = j;
			int x = (i - 2) / 2;
			char tile = (b->grid)[(y - 1) * b->width + (x - 1)];
			if (i == 0) {
				printf(" ");
			} else if (i == 1 && j > 0) {
				printf("%c", j + 'a' - 1);
			} else if (i == 2) {
				printf("|");
			} else {
				if (i % 2 == 1) {
					printf(" ");
				} else {
					printf("%c", tile);
				}
			}
		}
		printf("\n");
	}
}

// record(msg, n, lo, hi) prints a message, tries to read an int from [lo, hi] and store it in n
// if unsuccessful, it prints a message and tries again
// effects: 
//	may modify n
//	prints a message
// requires: 
//	lo <= hi
//	n != NULL
static void record(const char *msg, int *n, int lo, int hi) {
	assert(n && lo <= hi);
	printf("%s (%d-%d)\n", msg, lo, hi);
	while (scanf("%d", n) != 1 || *n < lo || *n > hi) {
		printf("Fail, try again.\n");
		while (getchar() != '\n');
	}
}

// prints a help message
static void print_help(void) {
	printf("Enter commands in the form cxy, where\n");
	printf("\tc is 'f' (flag) or 'r' (reveal),\n");
	printf("\tx is the column identifier,\n");
	printf("\ty is the row identifier.\n");
	printf("Enter 'q' to quit.\n");
}

// an interactive minesweeper game
// effects: lots of i/o
int main(void) {
	srand(time(NULL));

	int height, width, num_mines;
	const int limit = 26;

	printf("Welcome to ASCII Minesweeper!\n");
	record("Enter board width", &width, 1, limit);
	record("Enter board height", &height, 1, limit);

	const int len = width * height;
	record("Enter mine count", &num_mines, 1, len);

	char grid[len];
	struct tile mines[num_mines];
	struct board b = {width, height, grid, num_mines, mines};

	// initialise board
	for (int i = 0; i < len; i++) {
		b.grid[i] = UNREVEALED;
	}
	generate_mines(&b);
	print_board(&b);
	print_help();

	char command;
	while (scanf(" %c", &command)) {
		if (command == 'f') {
			int x = getchar() - 'a' + 1;
			int y = getchar() - 'a' + 1;
			if (flag(&b, x, y)) {
				print_board(&b);
	  		} else {
				print_help();
	  		}
		} else if (command == 'r') {
	  		int x = getchar() - 'a' + 1;
			int y = getchar() - 'a' + 1;
			if (reveal(&b, x, y)) {
				print_board(&b);
				if (game_won(&b)) {
					printf("Congratulations, you won!\n");
					return 0;
				} else if (game_lost(&b)) {
					printf("RIP, try again!\n");
					return 0;
				}
	  		} else {
				print_help();
	  		}
		} else if (command == 'q') {
			return 0;
		} else {
			print_help();
		}
		while (getchar() != '\n');
	}
}
