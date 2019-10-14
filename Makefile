CFLAGS=`pkg-config --cflags --libs gtk+-3.0`

.PHONY: all

all: cli/cli gui/gui

cli/cli: src/minesweeper.c

gui/gui: src/minesweeper.c

clean:
	$(RM) cli/cli
	$(RM) gui/gui
