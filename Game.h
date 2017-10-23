#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#include "constants.h"
#include "Square.h"

// Handles game progress like a finite state machine
class Game
{
  public:
  Game();
  void progress( Square squares[] );
  void completeTurn();
  void reset( Square squares[] );
  int getState();

  private:
  GameStateFlag mCurrentState;
  GameStateFlag mNextStartState;
};

#endif
