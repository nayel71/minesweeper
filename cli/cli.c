#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cli.h"

// ANSI escape codes
static const char *erase_line = "\r\e[K";
static const char *go_up      = "\e[%dA";
static const char *red        = "\e[31;1m";
static const char *green      = "\e[32;1m";
static const char *blue       = "\e[34;1m";
static const char *normal     = "\e[0m";

static int mines_remaining;

void print_board(const struct board *b) {
	assert(b);
	printf("___|");
	for (int x = 1; x <= b->width; x++) {
		printf(x < 10 ? "__%d" : "_%d", x);
	}
	printf("\n");
	for (int y = 1; y <= b->height; y++) {
		printf("%3d|", y);
		for (int x = 1; x <= b->width; x++) {
			char label = b->grid[(y - 1) * b->width + x - 1];
			if (label == MINE) {
				printf("%s%3c%s", red, label, normal);
			} else if (label == FLAG) {
				printf("%s%3c%s", green, label, normal);
			} else {
				printf("%s%3c%s", blue, label, normal);
			}
		}
		printf("\n");
	}
}

void print_commands(void) {
	printf("Commands: f(lag)/r(eveal) [col] [row], or q(uit)");
	printf("\n\n%sMines Remaining: %d", erase_line, mines_remaining);
	printf(go_up, 1);
	printf("%scommand> ", erase_line);
}

static inline bool is_digit(char c) { return '0' <= c && c <= '9'; }

static inline void ignore_spaces(char *line) { while (*line == ' ') line++; }

static void parse_command(char *line, char *command, int *x, int *y) {
	assert(line && command && x && y);

	ignore_spaces(line);
	*command = *line; // *command should be the first non-space char in line

	while (line && !is_digit(*line)) { // ignore non-digit characters
		line++;
	}
	*x = atoi(line);

	while (line && is_digit(*line)) { // ignore the digits of *x
		line++;
	}
	*y = atoi(line); // atoi already ignores spaces
}

int play_cli(struct board *b) {
	mines_remaining = b->num_mines;
	print_board(b);
	print_commands();

	// continuously read and execute commands
	char *line = NULL;
	size_t n = 0;
	char command;
	int x, y;
	while (getline(&line, &n, stdin) > 0) {
		parse_command(line, &command, &x, &y);
		free(line);
		line = NULL;
		if (command == 'f') {
			if (flag(b, x, y)) {
				int index = (y - 1) * b->width + x - 1;
				if (b->grid[index] == FLAG) {
					mines_remaining--;
				} else {
					mines_remaining++;
				}
				printf(go_up, b->height + 3);
				print_board(b);
			} else {
				printf(go_up, 2);
			}
		} else if (command == 'r') {
			if (reveal(b, x, y)) {
				printf(go_up, b->height + 3);
				print_board(b);
				if (game_won(b)) {
					printf("\n\n%sWell Done\n", erase_line);
					return EXIT_SUCCESS;
				} else if (game_lost(b)) {
					printf("\n\n%sGame Over\n", erase_line);
					return EXIT_SUCCESS;
				}
			} else {
				printf(go_up, 2);
			}
		} else if (command == 'q') {
			printf("%sQuitting\n", erase_line);
			return EXIT_SUCCESS;
		} else {
			printf(go_up, 2);
		}
		print_commands();
	}

	// if getline failed, we still need to free line
	free(line); 
	return EXIT_FAILURE;
}
