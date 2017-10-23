#ifndef SQUARE_H
#define SQUARE_H

#include <SDL2/SDL.h>

#include "constants.h"

// Used to render and animate X's or O's at locations on the Tic Tac Toe grid
class Square
{
  public:
  static const int SQUARE_WIDTH = 100;
  static const int SQUARE_HEIGHT = 100;

  Square();
  void setPosition( int x, int y );
  bool handleEvent( SDL_Event* e, int gameState );
  void clear();
  void render();
  int getState();

  private:
  SDL_Point mPosition;
  SquareState mCurrentState; 
  SquareSprite mCurrentSprite;
  Uint32 mInitialTicks;
  Uint32 mFinalTicks;
};

#endif
