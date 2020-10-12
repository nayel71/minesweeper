import React from "react";
import "./styles.css";

let UNREVEALED = "🌿";
let FLAG = "🚩";
let MINE = "💥";
let DEAD = "😵";
let DIGITS = ["🌻", "1️⃣", "2️⃣", "3️⃣", "4️⃣", "5️⃣", "6️⃣", "7️⃣", "8️⃣"];

function listContains(l, x, y) {
  return l.some((m) => m[0] === x && m[1] === y);
}

class Minesweeper {
  constructor(width, height, mineCount) {
    this.width = width;
    this.height = height;
    this.mineCount = mineCount;
    this.revealedCount = 0;
    this.minesRemaining = mineCount;
    this.status = (
      <div id="status">
        {FLAG} {this.minesRemaining}
      </div>
    );
    this.gameOver = false;
    this.board = [];
    for (let y = 0; y < height; y++) {
      let row = [];
      for (let x = 0; x < width; x++) {
        row.push(UNREVEALED);
      }
      this.board.push(row);
    }
    this.generateMines();
    this.createButtons();
  }

  createButtons() {
    this.buttons = [];
    for (let y = 0; y < this.height; y++) {
      let row = [];
      for (let x = 0; x < this.width; x++) {
        let buttonID = y + this.width * x;
        row.push(
					<div key={buttonID}>
						<button
							className="Button"
							id={buttonID}
							onClick={this.updateButtons.bind(this, x, y)}
							onContextMenu={this.handleContextMenu.bind(this, x, y)}
						>
							{this.board[y][x]}
						</button>
					</div>
        );
      }
      this.buttons.push(
				<div className="Row" key={row[y].key}>
					{row}
				</div>
			);
    }
  }

  updateButtons(x, y) {
    // reveal square (x, y) and update all button texts
    this.reveal(x, y);
    for (let j = 0; j < this.height; j++) {
      for (let i = 0; i < this.width; i++) {
        let id = j + this.width * i;
        document.getElementById(id).textContent = this.board[j][i];
      }
    }
  }

  handleContextMenu(x, y) {
    // flag/unflag square (x, y)
    this.flag(x, y);
    let id = y + this.width * x;
    document.getElementById(id).textContent = this.board[y][x];
    document.getElementById("status").textContent = FLAG + this.minesRemaining;
  }

  generateMines() {
    // randomly generates mines
    this.mines = [];
    while (this.mines.length !== this.mineCount) {
      let x = Math.floor(Math.random() * this.width);
      let y = Math.floor(Math.random() * this.height);
      if (!listContains(this.mines, x, y)) {
        this.mines.push([x, y]);
      }
    }
  }

  flag(x, y) {
    // flags/unflags square (x, y)
    if (
      !this.gameOver &&
      0 <= x &&
      x < this.width &&
      0 <= y &&
      y < this.height
    ) {
      if (this.board[y][x] === UNREVEALED && this.minesRemaining > 0) {
        this.board[y][x] = FLAG;
        this.minesRemaining--;
      } else if (this.board[y][x] === FLAG) {
        this.board[y][x] = UNREVEALED;
        this.minesRemaining++;
      }
    }
  }

  neighbours(x, y) {
    // returns a list of neighbours of square (x, y)
    let lst = [];
    for (let b = -1; b <= 1; b++) {
      for (let a = -1; a <= 1; a++) {
        if (
          0 <= x + a &&
          x + a < this.width &&
          0 <= y + b &&
          y + b < this.height &&
          !listContains(lst, x + a, y + b)
        ) {
          lst.push([x + a, y + b]);
        }
      }
    }
    return lst;
  }

  countMines(x, y) {
    // returns the number of mines adjacent to square (x, y)
    let count = 0;
    for (let n of this.neighbours(x, y)) {
      if (listContains(this.mines, n[0], n[1])) {
        count++;
      }
    }
    return count;
  }

  reveal(x, y) {
    // reveals square (x, y)
    if (
      !this.gameOver &&
      0 <= x &&
      x < this.width &&
      0 <= y &&
      y < this.height &&
      this.board[y][x] === UNREVEALED
    ) {
      if (listContains(this.mines, x, y)) {
        this.gameOver = true;
        for (let m of this.mines) {
          if (this.board[m[1]][m[0]] !== FLAG) {
            this.board[m[1]][m[0]] = MINE;
          }
        }
        this.board[y][x] = DEAD;
        alert("Game Over");
      } else {
        this.board[y][x] = DIGITS[this.countMines(x, y)];
        this.revealedCount++;
        if (this.board[y][x] === DIGITS[0]) {
          for (let m of this.neighbours(x, y)) {
            this.reveal(m[0], m[1]);
          }
        }
        if (this.win()) {
          for (let m of this.mines) {
            this.board[m[1]][m[0]] = FLAG;
          }
          alert("You win!");
        }
      }
    }
  }

  win() {
    // returns true if game is won
    return this.revealedCount === this.width * this.height - this.mineCount;
  }
}

let game = new Minesweeper(20, 10, 30);

export default function App() {
  return (
    <div className="App">
      <h1>Minesweeper</h1>
      <div>{game.buttons}</div>
      <h4>{game.status}</h4>
    </div>
  );
}
