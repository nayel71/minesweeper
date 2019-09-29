#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "minesweeper.h"

static const char offset = 'a' - 1;

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
	printf("__|_");
	for (int x = 1; x <= b->width; x++) {
		printf("%c_", offset + x);
	}
	printf("\n");
	for (int y = 1; y <= b->height; y++) {
		printf("%2c|", offset + y);
		for (int x = 1; x <= b->width; x++) {
			printf("%2c", b->grid[(y - 1) * b->width + x - 1]);
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

	char command, x, y;
	while (scanf(" %c", &command)) {
		if (command == 'f') {
			scanf(" %c %c", &x, &y);
			if (flag(&b, x - offset, y - offset)) {
				print_board(&b);
	  		} else {
				print_help();
	  		}
		} else if (command == 'r') {
			scanf(" %c %c", &x, &y);
			if (reveal(&b, x - offset, y - offset)) {
				print_board(&b);
				if (game_won(&b)) {
					printf("Congratulations, you won!\n");
					return EXIT_SUCCESS;
				} else if (game_lost(&b)) {
					printf("RIP, try again!\n");
					return EXIT_SUCCESS;
				}
	  		} else {
				print_help();
	  		}
		} else if (command == 'q') {
			return EXIT_SUCCESS;
		} else {
			print_help();
		}
		while (getchar() != '\n');
	}

	return EXIT_FAILURE;
}
