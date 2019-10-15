CFLAGS=`pkg-config --cflags --libs gtk+-3.0`

src/main: cli/cli.c gui/gui.c src/minesweeper.c

clean:
	$(RM) src/main
