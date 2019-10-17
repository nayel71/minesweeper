# Minesweeper
A Minesweeper game in C

# Installation
Download the repository, open it in terminal and run `make`. To play, enter 

`src/main [gui|cli] [width] [height] [mine count]`

# Features
- The player chooses the grid dimensions and the number of mines.
- (GUI version) Colour-coded button labels.
- (GUI version) Left-clicking on a cell reveals it, right-clicking on a cell flags/unflags it.
- (Command-line version) Whitespace is ignored when entering commands.
- (Command-line version) At most one command may be entered at a time. (This helps prevent mistakes.)
- (Command-line version) Entering an invalid command displays instructions. (They are also displayed at the start.)

# GUI Screenshots
Game Start                              | Mid-Game                             
:--------------------------------------:|:------------------------------------:
![GUI Start](screenshots/gui_start.png) |![GUI Play](screenshots/gui_play.png) 
Game Won                                | Game Lost                           
![GUI Won](screenshots/gui_won.png)     |![GUI Lost](screenshots/gui_lost.png)  
