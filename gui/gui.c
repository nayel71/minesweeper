#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "gui.h"

const int LEFT_CLICK = 1;
const int RIGHT_CLICK = 3;

GtkWidget *window;
GtkWidget *grid;
GtkWidget **buttons;
GtkWidget *quit_button;

static int mines_remaining;

static const char *markup_format = "<span foreground=\"%s\"><big><b>%s</b></big></span>";

// updates the colour and text of label
static void update_markup(GtkWidget *label, const char *colour, const gchar *label_text) {
	gchar *markup = g_markup_printf_escaped(markup_format, colour, label_text);
	gtk_label_set_markup(GTK_LABEL(label), markup);
	g_free(markup);
}

void click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	struct args *data = (struct args *)user_data;

	if (event->type == GDK_BUTTON_PRESS && event->button == LEFT_CLICK) {
		// left click = reveal
		g_signal_connect((GtkButton *)widget, "button-press-event", G_CALLBACK(reveal), data);

		int grid_size = data->b->width * data->b->height;

		if (reveal(data->b, data->x, data->y)) {
			// since reveal is recursive, we may need to update the entire grid
			for (int i = 0; i < grid_size; i++) {
				// first make the revealed cells unclickable
				if (data->b->grid[i] != UNREVEALED && data->b->grid[i] != FLAG) {
					gtk_widget_set_sensitive(buttons[i], FALSE);
				}

				// next add proper markups
				if (data->b->grid[i] == MINE) {
					gtk_widget_set_name(buttons[i], "mine");
				} else if (data->b->grid[i] == FLAG) {
					gtk_widget_set_name(buttons[i], "flag");
				} else {
					GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
					gchar *label_text = g_strdup_printf("%c", data->b->grid[i]);
					update_markup(label, "blue", label_text);
					g_free(label_text);
				}
			}
		}

		// check for game over
		if (game_won(data->b)) {
			// update quit button markup
			GtkWidget *label = gtk_bin_get_child(GTK_BIN(quit_button));
			update_markup(label, "green", "Well done!");

			// add proper markups and make buttons unclickable
			for (int i = 0; i < grid_size; i++) {
				if (data->b->grid[i] == FLAG) {
					gtk_widget_set_name(buttons[i], "flag");
				}
				gtk_widget_set_sensitive(buttons[i], FALSE);
			}

			free(data);
		} else if (game_lost(data->b)) {
			// update quit button markup
			GtkWidget *label = gtk_bin_get_child(GTK_BIN(quit_button));
			update_markup(label, "red", "Game Over");

			// add proper markups and make buttons unclickable
			for (int i = 0; i < grid_size; i++) {
				if (data->b->grid[i] == MINE) {
					gtk_widget_set_name(buttons[i], "mine");
				}
				gtk_widget_set_sensitive(buttons[i], FALSE);
			}

			free(data);
		}
	} else if (event->type == GDK_BUTTON_PRESS && event->button == RIGHT_CLICK) {
		// right click = flag
		g_signal_connect((GtkButton *)widget, "button-press-event", G_CALLBACK(flag), data);

		if (flag(data->b, data->x, data->y)) {
			int i = (data->y - 1) * data->b->width + data->x - 1;

			// update window title
			if (data->b->grid[i] == FLAG) {
				gtk_widget_set_name(buttons[i], "flag");
				mines_remaining--;
			} else {
				gtk_widget_set_name(buttons[i], "none");
				mines_remaining++;
			}
			const gchar *title = g_markup_printf_escaped("Minesweeper %d x %d (%d mine(s) remaining)", 
				data->b->width, data->b->height, mines_remaining);
			gtk_window_set_title(GTK_WINDOW(window), title);
		}
	}
}

// attach buttons to grid
static void attach_buttons(struct board *b) {
	assert(b);
	for (int y = 0; y < b->height; y++) {
		for (int x = 0; x < b->width; x++) {
			int i = y * b->width + x;

			gchar *label = g_strdup_printf("%c", b->grid[i]);
			buttons[i] = gtk_button_new_with_label(label);

			struct args *data = (struct args *)malloc(sizeof(struct args));
			data->b = b;
			data->x = x + 1;
			data->y = y + 1;

			g_signal_connect(buttons[i], "button-press-event", G_CALLBACK(click), data);
			gtk_grid_attach(GTK_GRID(grid), buttons[i], x, y, 1, 1);

			g_free(label);
		}
	}

	quit_button = gtk_button_new_with_label("Quit");
	GtkWidget *label = gtk_bin_get_child(GTK_BIN(quit_button));
	const gchar *label_text = gtk_button_get_label((GtkButton *)quit_button);
	gchar *markup = g_markup_printf_escaped(markup_format, "blue", label_text);
	gtk_label_set_markup(GTK_LABEL(label), markup);
	g_free(markup);

	g_signal_connect_swapped(quit_button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_grid_attach(GTK_GRID(grid), quit_button, 0, b->height, b->width, 1);
}

void activate(GtkApplication *app, gpointer user_data) {
	/* create a new window */
	window = gtk_application_window_new(app);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new();

	/* Pack the container in the window */
	gtk_container_add(GTK_CONTAINER(window), grid);

	// construct board from user data
	struct board *b = (struct board *)user_data;
	attach_buttons(b);

	// set window title
	mines_remaining = b->num_mines;
	const gchar *title = g_markup_printf_escaped("Minesweeper %d x %d (%d mine(s) remaining)", 
		b->width, b->height, mines_remaining);
	gtk_window_set_title(GTK_WINDOW(window), title);

	// add CSS button colours: MINE = red, FLAG = green, none = none
	GtkCssProvider *provider = gtk_css_provider_new();
	GdkDisplay *display = gdk_display_get_default();
	GdkScreen *screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	gtk_css_provider_load_from_data(
		GTK_CSS_PROVIDER(provider), 
		"#flag { background:green } #mine { background:red }", 
		-1, 
		NULL
	);

	/* Now that we are done packing our widgets, we show them all
	 * in one go, by calling gtk_widget_show_all() on the window.
	 * This call recursively calls gtk_widget_show() on all widgets
	 * that are contained in the window, directly or indirectly.
	 */
	gtk_widget_show_all(window);
}

int play_gui(int argc, char **argv, struct board *b) {
	// activate GTK application
	buttons = (GtkWidget **)malloc(b->width * b->height * sizeof(GtkWidget *));
	GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), b);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	// clean up
	free(buttons);

	return status;
}
