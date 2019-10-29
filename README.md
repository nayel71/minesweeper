# Minesweeper
A Minesweeper game in C

# Installation
Download the repository, open it in terminal and run `make`. To play, enter 

`src/main [gui|cli] [width] [height] [mine count]`

# Pre-requisites

- [GTK](https://www.gtk.org)
- A terminal supporting ANSI escape codes

# Features
- The player chooses the grid dimensions and the number of mines.
- (GUI version) Colour-coded button labels.
- (GUI version) Left-clicking on a cell reveals it, right-clicking on a cell flags/unflags it.
- (CLI version) Separate terminal.
- (CLI version) Spaces and invalid commands are ignored.

# CLI Screenshots
![CLI Start](screenshots/cli_start.png)|![CLI Play](screenshots/cli_play.png)
:-------------------------------------:|:-----------------------------------:
![CLI Won](screenshots/cli_won.png)    |![CLI Lost](screenshots/cli_lost.png)

# GUI Screenshots
![GUI Start](screenshots/gui_start.png)

![GUI Play](screenshots/gui_play.png)

![GUI Won](screenshots/gui_won.png)

![GUI Lost](screenshots/gui_lost.png)
