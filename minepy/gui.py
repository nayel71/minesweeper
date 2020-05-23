from minesweeper import Minesweeper, UNREVEALED, MINE, FLAG
import tkinter as tk

class GUI(Minesweeper):
    """Minesweeper GUI version."""

    def __init__(self, width, height, mine_count):
        super().__init__(width, height, mine_count)
        self.window = tk.Tk()
        self.window.resizable(False, False)

        self.buttons = []
        self.attach_buttons()
        self.default_bg = self.buttons[0].cget("bg")

        self.status = f"Minesweeper {self.width} x {self.height}" + " ({} mine(s) remaining){}"
        self.update(0, 0)
        self.window.mainloop()


    def add_button(self, row_frame, x, y):
        """Create and add button for square (x, y) to row_frame."""
        button = tk.Button(row_frame, text=self.board[y][x],
                           height=1, width=1,
                           fg="blue", bd=1,
                           font="Helvetica 12 bold")
        self.buttons.append(button)
        button.bind("<Button-1>", lambda event: self.left_click(x, y))
        button.bind("<Button-2>", lambda event: self.right_click(x, y))
        button.bind("<Button-3>", lambda event: self.right_click(x, y))
        button.pack(side="left")


    def attach_buttons(self):
        """Attach buttons to window."""
        for y in range(self.height):
            row_frame = tk.Frame(self.window)
            row_frame.pack(side="top")
            for x in range(self.width):
                self.add_button(row_frame, x, y)

        self.buttons.append(
            tk.Button(self.window,
                      text="Restart",
                      bg="red", fg="white", bd=1,
                      font="Helvetica 12 bold",
                      command=lambda: self.restart()
            ).pack(fill=tk.X, side="top")
        )


    def update(self, x=None, y=None):
        """Update game status and button configurations."""
        if x is not None and y is not None:
            pos = y * self.width + x
            if self.board[y][x] == FLAG:
                self.buttons[pos].config(bg="green")
            else:
                self.buttons[pos].config(bg=self.default_bg)
        else:
            for j in range(self.height):
                for i in range(self.width):
                    pos = j * self.width + i
                    if self.board[j][i] == MINE:
                        self.buttons[pos].config(bg="red")
                    elif self.board[j][i] == FLAG:
                        self.buttons[pos].config(bg="green")
                    elif self.board[j][i] != UNREVEALED and self.buttons[pos]["text"] != self.board[j][i]:
                        self.buttons[pos].config(text=self.board[j][i], bg=self.default_bg)

        if self.game_won():
            self.window.title(self.status.format(self.mines_remaining, " - You win!"))
        elif self.game_lost():
            self.window.title(self.status.format(self.mines_remaining, " - Game Over"))
        else:
            self.window.title(self.status.format(self.mines_remaining, ""))


    def left_click(self, x, y):
        """Handle left click on button (x, y)."""
        if not self.game_lost():
            self.reveal(x, y)
            self.update()


    def right_click(self, x, y):
        """Handle right click on button (x, y)."""
        if not self.game_lost():
            self.flag(x, y)
            self.update(x, y)


    def restart(self):
        """Restart game."""
        self.window.destroy()
        self.__init__(self.width, self.height, self.mine_count)
