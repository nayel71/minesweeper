import Minesweeper from "./Minesweeper";
jest.mock("./Minesweeper");

beforeEach(() => {
  Minesweeper.mockClear();
});

it("class constructed", () => {
  const game = new Minesweeper(10, 11, 12);
  expect(Minesweeper).toHaveBeenCalled();
  expect(Minesweeper.mock.calls.length).toBe(1);
  expect(Minesweeper.mock.calls[0][0]).toBe(10);
  expect(Minesweeper.mock.calls[0][1]).toBe(11);
  expect(Minesweeper.mock.calls[0][2]).toBe(12);
});

it("methods called", () => {
  expect(Minesweeper).not.toHaveBeenCalled();

  const game = new Minesweeper();
  const mockGame = Minesweeper.mock.instances[0];

  game.createButtons();
  const mockCreateButtons = mockGame.createButtons;
  expect(mockCreateButtons).toHaveBeenCalledTimes(1);

  game.updateButtons();
  const mockUpdateButtons = mockGame.updateButtons;
  expect(mockUpdateButtons).toHaveBeenCalledTimes(1);

  game.handleContextMenu();
  const mockHandleContextMenu = mockGame.handleContextMenu;
  expect(mockHandleContextMenu).toHaveBeenCalledTimes(1);

  game.generateMines();
  const mockGenerateMines = mockGame.generateMines;
  expect(mockGenerateMines).toHaveBeenCalledTimes(1);

  game.flag();
  const mockFlag = mockGame.flag;
  expect(mockFlag).toHaveBeenCalledTimes(1);

  game.reveal();
  const mockReveal = mockGame.reveal;
  expect(mockReveal).toHaveBeenCalledTimes(1);
});
