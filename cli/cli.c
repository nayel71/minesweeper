#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../src/minesweeper.h"
#include "gui.h"

// print_board(b) displays the minesweeper board *b in a nice format
// requires: *b is a valid board
static void print_board(const struct board *b) {
	assert(b);
	printf("___|");
	for (int x = 1; x <= b->width; x++) {
		printf("_%02d", x);
	}
	printf("\n");
	for (int y = 1; y <= b->height; y++) {
		printf("%3d|", y);
		for (int x = 1; x <= b->width; x++) {
			printf("%3c", b->grid[(y - 1) * b->width + x - 1]);
		}
		printf("\n");
	}
}

// prints a list of commands
static void print_commands(void) {
	printf("Enter commands in the form c x y, where\n");
	printf("\tc is 'f' (flag) or 'r' (reveal),\n");
	printf("\tx is the column identifier,\n");
	printf("\ty is the row identifier.\n");
	printf("Enter 'q' to quit.\n");
}

// prints usage information
static void print_help(const char *progname) {
	fprintf(stderr, "usage: %s width height mine-count\n", progname);
}

// an interactive minesweeper game
// effects: lots of i/o
int main(int argc, char **argv) {
	if (argc < 4) {
		print_help(argv[0]);
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int num_mines = atoi(argv[3]);

	const int len = width * height;

	char grid[len];
	struct tile mines[num_mines];
	struct board b = {width, height, grid, num_mines, mines};

	// initialise board
	for (int i = 0; i < len; i++) {
		b.grid[i] = UNREVEALED;
	}
	generate_mines(&b);
	print_board(&b);
	print_commands();

	char command;
	int x, y;
	while (scanf(" %c", &command)) {
		if (command == 'f') {
			scanf("%d%d", &x, &y);
			if (flag(&b, x, y)) {
				print_board(&b);
	  		} else {
				print_commands();
	  		}
		} else if (command == 'r') {
			scanf("%d%d", &x, &y);
			if (reveal(&b, x, y)) {
				print_board(&b);
				if (game_won(&b)) {
					printf("Well done!\n");
					return EXIT_SUCCESS;
				} else if (game_lost(&b)) {
					printf("Game Over\n");
					return EXIT_SUCCESS;
				}
	  		} else {
				print_commands();
	  		}
		} else if (command == 'q') {
			return EXIT_SUCCESS;
		} else {
			print_commands();
		}
		while (getchar() != '\n');
	}

	return EXIT_FAILURE;
}
