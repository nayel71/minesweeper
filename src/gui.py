from minesweeper import Minesweeper
import sys
import tkinter as tk

class GUI(Minesweeper):
    def __init__(self, width, height, mine_count):
        super().__init__(width, height, mine_count)
        self.window = tk.Tk()
        self.window.title(f"Minesweeper {self.width} x {self.height} ({self.mines_remaining} mine(s) remaining)")
        self.window.resizable(False, False)

        self.buttons = []
        self.attach_buttons()
        self.default_bg = self.buttons[0].cget("bg")

        self.window.mainloop()


    def add_button(self, row_frame, label, x, y):
        button = tk.Button(row_frame, text=label,
                          height=1, width=1,
                          fg="blue", bd=1,
                          font="Helvetica 12 bold")
        self.buttons.append(button)
        button.bind("<Button-1>", lambda event:self.left_click(x, y))
        button.bind("<Button-3>", lambda event:self.right_click(x, y))
        button.pack(side="left")


    def attach_buttons(self):
        for y in range(self.height):
            row_frame = tk.Frame(self.window)
            row_frame.pack(side="top")
            for x in range(self.width):
                self.add_button(row_frame, self.board[y][x], x, y)

        self.buttons.append(
            tk.Button(self.window,
                      text="Restart",
                      bg="red", fg="white", bd=1,
                      font="Helvetica 12 bold",
                      command=lambda: self.restart()
            ).pack(fill=tk.X, side="top")
        )


    def update(self, x=None, y=None):
        self.window.title(f"Minesweeper {self.width} x {self.height} "
                          f"({self.mines_remaining} mine(s) remaining)")
        if x is not None and y is not None:
            pos = y * self.width + x
            if self.board[y][x] == Minesweeper.MINE:
                self.buttons[pos].config(bg="red")
            elif self.board[y][x] == Minesweeper.FLAG:
                self.buttons[pos].config(bg="green")
            else:
                self.buttons[pos].config(text=self.board[y][x], bg=self.default_bg)
        else:
            for j in range(self.height):
                for i in range(self.width):
                    pos = j * self.width + i
                    if self.board[j][i] == Minesweeper.MINE:
                        self.buttons[pos].config(bg="red")
                    elif self.board[j][i] == Minesweeper.FLAG:
                        self.buttons[pos].config(bg="green")
                    else:
                        self.buttons[pos].config(text=self.board[j][i], bg=self.default_bg)

        if self.game_won():
            self.window.title(f"Minesweeper {self.width} x {self.height} "
                              f"({self.mines_remaining} mine(s) remaining) - You win!")
        elif self.game_lost():
            self.window.title(f"Minesweeper {self.width} x {self.height} "
                              f"({self.mines_remaining} mine(s) remaining) - Game Over")


    def left_click(self, x, y):
        if self.game_over:
            return

        self.reveal(x, y)
        self.update()


    def right_click(self, x, y):
        if self.game_over:
            return

        self.flag(x, y)
        self.update(x, y)


    def restart(self):
        self.window.destroy()
        self.__init__(self.width, self.height, self.mine_count)


if __name__ == "__main__":
    def print_help_and_abort():
        print(f"usage: {sys.argv[0]} width height mine-count")
        print("1 <= width <= 99")
        print("1 <= height <= 99")
        print("1 <= mine-count <= width * height")

    try:
        width, height, mine_count = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
        # check bounds and keep board size reasonable
        if width < 1 or width > 99 or height < 1 or height > 99 or mine_count < 1 or mine_count > width * height:
            print_help_and_abort()
        else:
            GUI(width, height, mine_count)
    except IndexError:
        print("Could not launch GUI.")
