# Minesweeper
A Minesweeper game

## Features
- The player chooses the grid dimensions and the number of mines.
- **GUI:** Left-clicking on a cell reveals it, right-clicking on a cell flags/unflags it. 
- **CLI:** Separate terminal; extra spaces and invalid commands are ignored.
- **GUI (Python):** `Restart` button

## Installation
- `git clone https://github.com/nayel71/minesweeper.git`
- `cd minesweeper`
- `make` (C version)

## Starting the Game
- **C Version:** `./main [gui|cli] [width] [height] [mine count]`
- **Python Version:** `python3 minepy [gui|cli] [width] [height] [mine count]`

## Requirements
- **C Version:** [GTK](https://www.gtk.org)
- **Python Version:** [Tkinter](https://docs.python.org/3/library/tkinter.html)

## Screenshots
![](screenshots/cli.png)
![](screenshots/c_gui.png)
![](screenshots/py_gui.png)
