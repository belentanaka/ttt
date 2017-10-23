#ifndef CONSTANTS_H
#define CONSTANTS_H

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Dimensions of Tic Tac Toe grid
const int TOTAL_ROWS = 3;
const int TOTAL_COLS = 3;
const int TOTAL_SQUARES = TOTAL_ROWS * TOTAL_COLS;
const int WIN_NUMBER = 3; // Number of X's or O's in a row to win

// States used for rendering
enum SquareState
{
  SQUARE_STATE_BLANK,
  SQUARE_STATE_X,
  SQUARE_STATE_O
};

// Tells squares what to render
enum SquareSprite
{
  SQUARE_SPRITE_BLANK = 0,
  SQUARE_SPRITE_X = 1,
  SQUARE_SPRITE_O = 5,
  SQUARE_SPRITE_COUNT = 9
};

// Tell game what to do
enum GameStateFlag
{
  STATE_X_TURN,
  STATE_O_TURN,
  STATE_X_WIN,
  STATE_O_WIN,
  STATE_DRAW,
};

#endif
