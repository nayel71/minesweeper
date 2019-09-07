#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "minesweeper.h"

// print_board(b) displays the minesweeper board b in a nice format
// requires: b is a valid board
static void print_board(const struct board *b) {
	assert(b);
	int height = b->height + 1;
	int width = 2 * b->width + 4;
	for (int i = 0; i < width; i++) {
		if (i == 2) {
			printf("|");
		} else {
			if (i % 2 == 0 && i > 1) {
				printf("%c", 'a' - 1 + (i-2)/2);
			} else {
				printf("_");
			}
		}
	}
	printf("\n");
	for (int j = 1; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int y = j;
			int x = (i-2)/2;
			char tile = (b->grid)[(y-1) * b->width + (x-1)];
			if (i == 0) {
				printf(" ");
			} else if (i == 1 && j > 0) {
				printf("%c", j + 'a' - 1);
			} else if (i == 2) {
				printf("|");
			} else {
				if (i % 2 == 1) printf(" ");
				else printf("%c", tile);
			}
		}
		printf("\n");
	}
}

// record(x, a, b) tries to read an int from the range [a,b] and store it in x
// if unsuccessful, it prints a message and tries again
// effects: may print a message
// requires: 
//	a <= b
//	x != NULL
static void record(int *x, int a, int b) {
	assert(x && a <= b);
	while (scanf("%d", x) != 1 || *x < a || *x > b) {
		printf("Fail, try again.\n");
		while (getchar() != '\n');
	}
}

// prints a help message
static void print_help(void) {
	printf("Enter commands in the form cxy, where c is 'f' (flag) or 'r' (reveal),\n");
	printf("x is the column identifier and y is the row identifier.\n");
}

// an interactive minesweeper game
// effects: lots of i/o
int main(void) {
	srand(time(NULL));

	int height, width, num_mines;
	printf("Welcome to ASCII Minesweeper!\n");
	
	printf("Enter board width (1-26)\n");
	record(&width, 1, 26);
	
	printf("Enter board height (1-26)\n");
	record(&height, 1, 26);
	
	int len = width * height;
	printf("Enter mine count (1-%d)\n", len);
	record(&num_mines, 1, len);

	char grid[len];
	struct square mines[num_mines];
	struct board b = {width, height, grid, num_mines, mines};
	for (int i = 0; i < len; ++i) {
		b.grid[i] = ' '; 
	}
	for (int i = 0; i < num_mines; i++) {
		b.mines[i].x = 1 + rand() % width;
		b.mines[i].y = 1 + rand() % height;
		for (int j = i-1; j >= 0; j--) {
			if (b.mines[i].x == b.mines[j].x && b.mines[i].y == b.mines[j].y) {
				i--;
				break;
			}
		}
	}

	print_board(&b);
	print_help();

	char command;
	char x1, y1;
	while (1) {
		scanf(" %c%c%c", &command, &x1, &y1);
		if (command == 'f') {
			int x = x1 - 'a' + 1;
			int y = y1 - 'a' + 1;
			if (flag(&b, x, y)) {
				print_board(&b);
	  		} else {
				print_help();
	  		}
		} else if (command == 'r') {
	  		int x = x1 - 'a' + 1;
			int y = y1 - 'a' + 1;
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
		} else {
			print_help();
			while (getchar() != '\n');
		}
	}
}
