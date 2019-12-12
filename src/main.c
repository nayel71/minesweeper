#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/gui.h"
#include "../include/cli.h"

// prints usage information
static void print_help_and_abort(const char *progname) {
	fprintf(stderr, "usage: %s [cli|gui] width height mine-count\n", progname);
	fprintf(stderr, "1 <= width <= 99\n");
	fprintf(stderr, "1 <= height <= 99\n");
	fprintf(stderr, "1 <= mine-count <= width * height\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	if (argc < 5) {
		print_help_and_abort(argv[0]);
	}

	srand(time(NULL));

	// read command line arguments
	const int width = atoi(argv[2]);
	const int height = atoi(argv[3]);
	const int num_mines = atoi(argv[4]);
	const int len = width * height;

	// check bounds and keep board size reasonable
	if (width < 1 || width > 99 || height < 1 || height > 99 || num_mines < 1 || num_mines > len) {
		print_help_and_abort(argv[0]);
	}

	// initialise
	char grid[len];
	struct tile mines[num_mines];
	struct board b = {width, height, grid, num_mines, mines, num_mines, 0};

	for (int i = 0; i < len; i++) {
		b.grid[i] = UNREVEALED;
	}

	generate_mines(&b);

	if (strcmp(argv[1], "gui") == 0) {
		return play_gui(argc - 5, argv + 5, &b);
	} else if (strcmp(argv[1], "cli") == 0) {
		return play_cli(&b);
	}

	print_help_and_abort(argv[0]);
}
