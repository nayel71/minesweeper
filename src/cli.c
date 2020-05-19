#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../include/cli.h"

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
			printf("%3c", b->grid[(y - 1) * b->width + x - 1]);
		}
		printf("\n");
	}
	printf("Mines Remaining: %d\n", b->mines_remaining);
}

void print_commands(void) {
	printf("Commands: f(lag)/r(eveal) [col] [row], or q(uit)\n");
	printf("command> ");
}

static inline bool is_digit(char c) { return '0' <= c && c <= '9'; }

static void parse_command(char *line, int n, char *command, int *x, int *y) {
	assert(line && command && x && y);

	int i = 0;
	for (; i < n && line[i] == ' '; i++);
	*command = line[i];

	for (; i < n && !is_digit(line[i]); i++);
	*x = atoi(line + i);

	for (; i < n && is_digit(line[i]); i++);
	*y = atoi(line + i);
}

int play_cli(struct board *b) {
	print_board(b);
	print_commands();

	// continuously read and execute commands
	char *line = NULL;
	size_t n = 0;
	char command;
	int x, y;
	while (getline(&line, &n, stdin) > 0) {
		parse_command(line, n, &command, &x, &y);
		switch (command) {
		case 'f':
		case 'F':
			flag(b, x, y);
			print_board(b);
			break;
		case 'r':
		case 'R':
			reveal(b, x, y);
			print_board(b);
			if (game_won(b)) {
				printf("Well Done\n");
				free(line);
				return EXIT_SUCCESS;
			} else if (game_lost(b)) {
				printf("Game Over\n");
				free(line);
				return EXIT_SUCCESS;
			}
			break;
		case 'q':
		case 'Q':
			printf("Quitting\n");
			free(line);
			return EXIT_SUCCESS;
		default:
			print_board(b);
		}
		print_commands();
	}

	free(line); 
	return EXIT_FAILURE;
}
