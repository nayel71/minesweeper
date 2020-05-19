from minesweeper import Minesweeper
import sys

class CLI(Minesweeper):
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
        s = "___|"
        for x in range(self.width):
            if x < 9:
                s += f"__{x + 1}"
            else:
                s += f"_{x + 1}"

        for row_num, row in enumerate(self.board):
            if row_num < 9:
                s += f"\n  {row_num + 1}|  "
            else:
                s += f"\n {row_num + 1}|  "
            s += "  ".join(str(e) for e in row)

        s += f"\nMines Remaining: {self.mines_remaining}"
        return s


    def print_commands(self):
        print("Commands: f(lag)/r(eveal) [col] [row], or q(uit)")


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
            CLI(width, height, mine_count)
    except IndexError:
        print("Could not launch CLI.")
