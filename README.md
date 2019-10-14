# Minesweeper
A Minesweeper game in C

# Installation
Download the repository, open it in terminal and run `make`. 

# Gameplay
On the terminal, enter 
- `cli/cli [width] [height] [mine count]` (command-line version), or 
- `gui/gui [width] [height] [mine count]` (GUI version)

# Features
- The player chooses the grid dimensions and the number of mines.
- (GUI version) Colour-coded button labels.
- (GUI version) Left-clicking on a cell reveals it, right-clicking on a cell flags/unflags it.
- (Command-line version) Whitespace is ignored when entering commands.
- (Command-line version) At most one command may be entered at a time. (This helps prevent mistakes.)
- (Command-line version) Entering an invalid command displays instructions. (They are also displayed at the start.)

# GUI Screenshots
(See `examples/cli.txt` for a CLI example.

![GUI Play](examples/gui_play.png)

![GUI Lost](examples/gui_lost.png)

![GUI Won](examples/gui_won.png)
