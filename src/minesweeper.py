#!/usr/bin/env python

import random

class Minesweeper:
    """Minesweeper game base class."""

    UNREVEALED = " "
    FLAG = "F"
    MINE = "X"

    def __init__(self, width, height, mine_count):
        self.width = width
        self.height = height
        self.mine_count = mine_count

        self.revealed_count = 0
        self.mines_remaining = mine_count
        self.game_over = False

        self.board = []
        for y in range(self.height):
            row = []
            for x in range(self.width):
                row.append(Minesweeper.UNREVEALED)

            self.board.append(row)

        self.mines = set()
        self.generate_mines()


    def generate_mines(self):
        """Randomly generate mines."""
        while len(self.mines) != self.mine_count:
            mine_x = random.randint(0, self.width - 1)
            mine_y = random.randint(0, self.height - 1)
            self.mines.add((mine_x, mine_y))


    def flag(self, x, y):
        """Flag/unflag square (x, y)."""
        if x in range(self.width) and y in range(self.height):
            if self.board[y][x] == Minesweeper.UNREVEALED:
                self.board[y][x] = Minesweeper.FLAG
                self.mines_remaining -= 1
            elif self.board[y][x] == Minesweeper.FLAG:
                self.board[y][x] = Minesweeper.UNREVEALED
                self.mines_remaining += 1


    def neighbours(self, x, y):
        """Return a list of neighbours of square (x, y)."""
        return ((a, b) for a in (x - 1, x, x + 1) for b in (y - 1, y, y + 1))


    def count_mines(self, x, y):
        """Return the number of mines adjacent to square (x, y)."""
        count = 0

        for (i, j) in self.neighbours(x, y):
            if (i, j) in self.mines:
                count += 1

        return count


    def reveal(self, x, y):
        """Reveal square (x, y)."""
        if x in range(self.width) and y in range(self.height) and self.board[y][x] == Minesweeper.UNREVEALED:
            if (x, y) in self.mines:
                self.game_over = True
                self.board[y][x] = Minesweeper.MINE

                for (i, j) in self.mines:
                    if self.board[j][i] != Minesweeper.FLAG:
                        self.board[j][i] = Minesweeper.MINE
            else:
                self.board[y][x] = self.count_mines(x, y)
                self.revealed_count += 1

                if self.board[y][x] == 0:
                    for (i, j) in self.neighbours(x, y):
                        self.reveal(i, j)

                if self.game_won():
                    for (i, j) in self.mines:
                        self.board[j][i] = Minesweeper.FLAG


    def game_won(self):
        """Return true if game is won."""
        return self.revealed_count == self.width * self.height - self.mine_count


    def game_lost(self):
        """Return true if game is lost."""
        return self.game_over
