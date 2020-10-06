#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "minesweeper.h"

extern const int LEFT_CLICK;
extern const int RIGHT_CLICK;

extern GtkWidget *window;
extern GtkWidget *grid;
extern GtkWidget **buttons;
extern GtkWidget *quit_button;

// struct for passing to the click callback function
struct args {
	struct board *b;
	int x;
	int y;
};

// callback function to handle button clicks
void click(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

// callback function to activate the GUI
void activate(GtkApplication *app, gpointer user_data);

// starts a GUI-based minesweeper game
int play_gui(int argc, char **argv, struct board *b);

#endif /* GUI_H */
