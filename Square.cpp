#include <SDL2/SDL.h>

#include "constants.h"
#include "textures.h"
#include "Square.h"

// Initialize member variables 
Square::Square()
{
  mPosition.x = 0;
  mPosition.y = 0;

  mCurrentState = SQUARE_STATE_BLANK;
  mCurrentSprite = SQUARE_SPRITE_BLANK;
  mInitialTicks = SDL_GetTicks();
  mFinalTicks = mInitialTicks;
}

// Sets (x,y) position on screen
void Square::setPosition( int x, int y )
{
  mPosition.x = x;
  mPosition.y = y;
}

// Detects and responds to mouse clicks
bool Square::handleEvent( SDL_Event* e, int gameState )
{
  bool turnComplete = false;

  // Detect mouse click
  if( ( mCurrentState == SQUARE_STATE_BLANK ) && ( e->type == SDL_MOUSEBUTTONUP ) )
  {
    int x, y;
    // Get position where mouse clicked
    SDL_GetMouseState( &x, &y );

    // Detect if mouse clicked on square
    if( ( x > mPosition.x ) && ( x < mPosition.x + SQUARE_WIDTH ) && ( y > mPosition.y ) && ( y < mPosition.y + SQUARE_HEIGHT ) )
    {
      // Draw X or O depending on who's turn it is

      if( gameState == STATE_X_TURN )
      {
	mCurrentState = SQUARE_STATE_X;
	mCurrentSprite = SQUARE_SPRITE_X;
	turnComplete = true;
      }
      else if( gameState == STATE_O_TURN )
      {
	mCurrentState = SQUARE_STATE_O;
	mCurrentSprite = SQUARE_SPRITE_O;
	turnComplete = true;
      }
    }
  }

  // Return true if X or O completed turn
  return turnComplete;
}

// Reset member variables (for new game)
void Square::clear()
{
  mCurrentState = SQUARE_STATE_BLANK;
  mCurrentSprite = SQUARE_SPRITE_BLANK;
  mInitialTicks = SDL_GetTicks();
  mFinalTicks = mInitialTicks;
}

// Render X or O at location depending on state
void Square::render()
{
  if( mCurrentState == SQUARE_STATE_X )
  {
    gSquareSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSquareSpriteClips[ mCurrentSprite + ( ( mFinalTicks - mInitialTicks ) / 100 ) ] );
    
    // Handle timer used for X animation
    mFinalTicks = SDL_GetTicks();
    if( ( mFinalTicks - mInitialTicks ) / 100 >= 4 )
    {
      mInitialTicks = mFinalTicks;
    }
  }
  else if( mCurrentState == SQUARE_STATE_O )
  {
    gSquareSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSquareSpriteClips[ mCurrentSprite + ( ( mFinalTicks - mInitialTicks ) / 100 ) ] );

    // Handle timer used for O animation
    mFinalTicks = SDL_GetTicks();
    if( ( mFinalTicks - mInitialTicks ) / 100 >= 4 )
    {
      mInitialTicks = mFinalTicks;
    }
  }
}

// Access state
int Square::getState()
{
  return mCurrentState;
}
