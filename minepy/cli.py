from minesweeper import Minesweeper

class CLI(Minesweeper):
    """Minesweeper CLI version."""

    def __init__(self, width, height, mine_count):
        super().__init__(width, height, mine_count)
        while True:
            print(self)
            self.print_commands()
            line = input("command> ")
            tokens = line.split()
            command = tokens[0]

            if command[0] == "r" or command[0] == "R":
                x = int(tokens[1]) - 1
                y = int(tokens[2]) - 1
                self.reveal(x, y)

                if self.game_won():
                    print(self)
                    print("You win!")
                    return
                elif self.game_lost():
                    print(self)
                    print("Game Over")
                    return
            elif command[0] == "f" or command[0] == "F":
                x = int(tokens[1]) - 1
                y = int(tokens[2]) - 1
                self.flag(x, y)
            elif command[0] == "q" or command[0] == "Q":
                print("Quitting...")
                return


    def __str__(self):
        s = "___|" + "".join(f"__{x + 1}" if x < 9 else f"_{x + 1}" for x in range(self.width))

        for num, row in enumerate(self.board):
            s += (f"\n  {num + 1}|  " if num < 9 else f"\n {num + 1}|  ") + "  ".join(str(e) for e in row)

        s += f"\nMines Remaining: {self.mines_remaining}"
        return s


    def print_commands(self):
        """Print a list of commands."""
        print("Commands: f(lag)/r(eveal) [col] [row], or q(uit)")
