#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL2/SDL.h>

#include "constants.h"
#include "Square.h"

// Handles game progress like a finite state machine
class GameState
{
  public:
  GameState();
  void progress( Square squares[] );
  void completeTurn();
  void reset( Square squares[] );
  int getState();

  private:
  GameStateFlag mCurrentState;
  GameStateFlag mNextStartState;
};

#endif
