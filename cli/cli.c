#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cli.h"

static const char *erase_line = "\r\e[K";
static const char *go_up = "\e[%dA";

void print_board(const struct board *b) {
	assert(b);
	printf("___|");
	for (int x = 1; x <= b->width; x++) {
		printf(x < 10? "__%d" : "_%02d", x);
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

void print_commands(void) {
	printf("Enter commands as f(lag)/r(eveal) [col] [row]. Enter q(uit) to quit.\n");
	printf("command> ");
}

static inline int is_digit(char c) { return '0' <= c && c <= '9'; }

static inline void ignore_spaces(char *line) { while (*line == ' ') line++; }

// reads two space-separated ints from line and stores them at x and y
static void read_two_ints(char *line, int *x, int *y) {
	while (line && !is_digit(*line)) { // ignore non-digit characters
		line++;
	}
	*x = atoi(line);
	while (line && is_digit(*line)) { // ignore the digits of *x
		line++;
	}
	ignore_spaces(line);
	*y = atoi(line);
}

int play_cli(struct board *b) {
	print_board(b);
	print_commands();

	// continuously read and execute commands
	char *line = NULL;
	size_t n = 0;
	int x, y;
	while (getline(&line, &n, stdin)) {
		ignore_spaces(line);
		char command = *line;
		if (command == 'f') {
			read_two_ints(line, &x, &y);
			if (flag(b, x, y)) {
				printf(go_up, b->height + 3);
				print_board(b);
				print_commands();
			} else {
				printf(go_up, 1);
			}
		} else if (command == 'r') {
			read_two_ints(line, &x, &y);
			if (reveal(b, x, y)) {
				printf(go_up, b->height + 3);
				print_board(b);
				print_commands();
				if (game_won(b)) {
					printf("%sWell Done\n", erase_line);
					return EXIT_SUCCESS;
				} else if (game_lost(b)) {
					printf("%sGame Over\n", erase_line);
					return EXIT_SUCCESS;
				}
			} else {
				printf(go_up, 1);
			}
		} else if (command == 'q') {
			return EXIT_SUCCESS;
		} else {
			printf(go_up, 1);
		}
		line = NULL;
		printf("%scommand> ", erase_line);
	}

	return EXIT_FAILURE;
}
