import React from "react";
import Minesweeper from "./Minesweeper"
import "./App.css";

export default function App() {
  let game = new Minesweeper(20, 10, 30);
  return (
    <div className="App">
      <h1>Minesweeper</h1>
      <div>{game.buttons}</div>
      <h4>{game.status}</h4>
    </div>
  );
}
