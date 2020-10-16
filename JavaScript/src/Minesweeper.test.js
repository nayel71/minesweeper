import Minesweeper from "./Minesweeper";
jest.mock("./Minesweeper");

beforeEach(() => {
  Minesweeper.mockClear();
});

it("class constructed", () => {
  const game = new Minesweeper();
  expect(Minesweeper).toHaveBeenCalled();
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
