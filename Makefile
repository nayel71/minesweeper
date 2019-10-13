CFLAGS=`pkg-config --cflags --libs gtk+-3.0`

.PHONY: all

all: play gui/gui

play: minesweeper.c

gui/gui: minesweeper.c
