#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"

const int LEFT_CLICK = 1;
const int RIGHT_CLICK = 3;

GtkWidget *window;
GtkWidget *grid;
GtkWidget **buttons;
GtkWidget *quit_button;

static const char *markup_format = "<span foreground=\"%s\"><big><b>%s</b></big></span>";

void click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	struct args *data = (struct args *)user_data;

	if (event->type == GDK_BUTTON_PRESS && event->button == LEFT_CLICK) {
		// left click = reveal
		g_signal_connect((GtkButton *)widget, "button-press-event", G_CALLBACK(reveal), data);

		int grid_size = data->b->width * data->b->height;

		if (reveal(data->b, data->x, data->y)) {
			// since reveal is recursive, we may need to update the entire grid
			for (int i = 0; i < grid_size; i++) {

				// first make revealed cells sensitive
				if ((data->b->grid)[i] != UNREVEALED && (data->b->grid)[i] != FLAG) {
					gtk_widget_set_sensitive(buttons[i], FALSE);
				}

				// next add proper markups
				if ((data->b->grid)[i] == MINE) {
					GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
					char label_text[2];
					label_text[0] = (data->b->grid)[i];
					label_text[1] = '\0';
					char *markup = g_markup_printf_escaped(markup_format, "red", label_text);
					gtk_label_set_markup(GTK_LABEL(label), markup);
					g_free(markup);
				} else if ((data->b->grid)[i] != FLAG) {
					GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
					char label_text[2];
					label_text[0] = (data->b->grid)[i];
					label_text[1] = '\0';
					char *markup = g_markup_printf_escaped(markup_format, "blue", label_text);
					gtk_label_set_markup(GTK_LABEL(label), markup);
					g_free(markup);
				}
			}
		}

		// check for game over
		if (game_won(data->b)) {
			GtkWidget *label = gtk_bin_get_child(GTK_BIN(quit_button));
			const char *label_text = "Well done!";
			char *markup = g_markup_printf_escaped(markup_format, "green", label_text);
			gtk_label_set_markup(GTK_LABEL(label), markup);
			g_free(markup);

			for (int i = 0; i < grid_size; i++) {
				gtk_widget_set_sensitive(buttons[i], FALSE);
			}

			free(data);
		} else if (game_lost(data->b)) {
			GtkWidget *label = gtk_bin_get_child(GTK_BIN(quit_button));
			const char *label_text = "Game Over";
			char *markup = g_markup_printf_escaped(markup_format, "red", label_text);
			gtk_label_set_markup(GTK_LABEL(label), markup);
			g_free(markup);

			for (int i = 0; i < grid_size; i++) {
				gtk_widget_set_sensitive(buttons[i], FALSE);
			}

			free(data);
		}
	} else if (event->type == GDK_BUTTON_PRESS && event->button == RIGHT_CLICK) {
		// right click = flag
		g_signal_connect((GtkButton *)widget, "button-press-event", G_CALLBACK(flag), data);

		if (flag(data->b, data->x, data->y)) {
			int i = (data->y - 1) * data->b->width + data->x - 1;

			GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
			char label_text[2];
			label_text[0] = (data->b->grid)[i];
			label_text[1] = '\0';
			char *markup = g_markup_printf_escaped(markup_format, "green", label_text);
			gtk_label_set_markup(GTK_LABEL(label), markup);
			g_free(markup);
		}
	}
}

// attach buttons to grid
static void attach_buttons(struct board *b) {
	for (int y = 0; y < b->height; y++) {
		for (int x = 0; x < b->width; x++) {
			int i = y * b->width + x;

			char label[2];
			label[0] = (b->grid)[i];
			label[1] = '\0';

			struct args *data = (struct args *)malloc(sizeof(struct args));
			data->b = b;
			data->x = x + 1;
			data->y = y + 1;

			buttons[i] = gtk_button_new_with_label(label);
			g_signal_connect(buttons[i], "button-press-event", G_CALLBACK(click), data);
			gtk_grid_attach(GTK_GRID(grid), buttons[i], x, y, 1, 1);
		}
	}

	quit_button = gtk_button_new_with_label("Quit");
	GtkWidget *label = gtk_bin_get_child(GTK_BIN(quit_button));
	const char *label_text = gtk_button_get_label((GtkButton *)quit_button);
	char *markup = g_markup_printf_escaped(markup_format, "blue", label_text);
	gtk_label_set_markup(GTK_LABEL(label), markup);
	g_free(markup);

	g_signal_connect_swapped(quit_button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_grid_attach(GTK_GRID(grid), quit_button, 0, b->height, b->width, 1);
}

void activate(GtkApplication *app, gpointer user_data) {
	/* create a new window, and set its title */
	window = gtk_application_window_new(app);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();

	/* Pack the container in the window */
	gtk_container_add(GTK_CONTAINER(window), grid);

	struct board *b = (struct board *)user_data;
	attach_buttons(b);

	/* Now that we are done packing our widgets, we show them all
	 * in one go, by calling gtk_widget_show_all() on the window.
	 * This call recursively calls gtk_widget_show() on all widgets
	 * that are contained in the window, directly or indirectly.
	 */
	gtk_widget_show_all(window);
}

// prints usage information
static void print_help(const char *progname) {
	fprintf(stderr, "usage: %s width height mine-count\n", progname);
	fprintf(stderr, "1 <= width\n");
	fprintf(stderr, "1 <= height\n");
	fprintf(stderr, "1 <= mine-count <= width * height\n");
}

int main(int argc, char **argv) {
	if (argc < 4) {
		print_help(argv[0]);
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	// read command line arguments
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int num_mines = atoi(argv[3]);

	if (width < 1 || height < 1 || num_mines < 1 || num_mines > width * height) {
		print_help(argv[0]);
		return EXIT_FAILURE;
	}

	// initialise
	const int len = height * width;
	char grid[len];
	buttons = (GtkWidget **)malloc(len * sizeof(GtkWidget *));
	struct tile mines[num_mines];
	struct board b = {width, height, grid, num_mines, mines};

	generate_mines(&b);

	for (int i = 0; i < len; i++) {
		b.grid[i] = UNREVEALED;
	}

	// activate GTK application
	GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), &b);
	int status = g_application_run(G_APPLICATION(app), argc - 3, argv + 3);
	g_object_unref(app);

	// clean up
	free(buttons);

	return status;
}
