CFLAGS=`pkg-config --cflags --libs gtk+-3.0`

.PHONY: all

all: play gtk/gui

play: minesweeper.c

gtk/gui: minesweeper.c
