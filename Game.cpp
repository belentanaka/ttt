#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

#include "constants.h"
#include "Square.h"
#include "Game.h"

// Initialize member variables
Game::Game()
{
  mCurrentState = STATE_X_TURN;
  mNextStartState = STATE_O_TURN;
}

// Detects if there is a victory or a draw
void Game::progress( Square squares[] )
{
  // If X's turn, check if X won
  if( mCurrentState == STATE_X_TURN )
  {
    for( int i = 0; i < TOTAL_SQUARES; i++ )
    {
      if( squares[ i ].getState() == SQUARE_STATE_X )
      {
	int n;

	// Check horizontal
	n = 1;
	for( int j = i + 1; j % TOTAL_COLS != 0; j++ )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_X )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_X_WIN;
	      return;
	    }
	  }
	}

	// Check vertical
	n = 1;
	for( int j = i + TOTAL_COLS; j < TOTAL_SQUARES; j += TOTAL_COLS )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_X )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_X_WIN;
	      return;
	    }
	  }
	}

	// Check diagonal ( / )
	n = 1;
	for( int j = i + TOTAL_COLS - 1; ( j % TOTAL_COLS != TOTAL_COLS - 1 ) && ( j < TOTAL_SQUARES ); j += TOTAL_COLS - 1 )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_X )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_X_WIN;
	      return;
	    }
	  } 
	}

	// Check diagonal ( \ )
	n = 1;
	for( int j = i + TOTAL_COLS + 1; ( j % TOTAL_COLS != 0 ) && ( j < TOTAL_SQUARES ); j += TOTAL_COLS + 1 )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_X )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_X_WIN;
	      return;
	    }
	  }
	}
      }
    }

    // If X did not win, it is O's turn
    mCurrentState = STATE_O_TURN;
  }
  // If O's turn, check if O won
  else if ( mCurrentState == STATE_O_TURN )
  {
    for( int i = 0; i < TOTAL_SQUARES; i++ )
    {
      if( squares[ i ].getState() == SQUARE_STATE_O )
      {
	int n;

	// Check horizontal
	n = 1;
	for( int j = i + 1; j % TOTAL_COLS != 0; j++ )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_O )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_O_WIN;
	      return;
	    }
	  }
	}

	// Check vertical
	n = 1;
	for( int j = i + TOTAL_COLS; j < TOTAL_SQUARES; j += TOTAL_COLS )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_O )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_O_WIN;
	      return;
	    }
	  }
	}

	// Check diagonal ( / )
	n = 1;
	for( int j = i + TOTAL_COLS - 1; ( j % TOTAL_COLS != TOTAL_COLS - 1 ) && ( j < TOTAL_SQUARES ); j += TOTAL_COLS - 1 )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_O )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_O_WIN;
	      return;
	    }
	  } 
	}

	// Check diagonal ( \ )
	n = 1;
	for( int j = i + TOTAL_COLS + 1; ( j % TOTAL_COLS != 0 ) && ( j < TOTAL_SQUARES ); j += TOTAL_COLS + 1 )
	{
	  if( squares[ j ].getState() != SQUARE_STATE_O )
	  {
	    break;
	  }
	  else
	  {
	    n++;
	    if( n == WIN_NUMBER )
	    {
	      mCurrentState = STATE_O_WIN;
	      return;
	    }
	  }
	}
      }
    }

    // If O did not win, it is X's turn
    mCurrentState = STATE_X_TURN;
  }

  // Check if there are blank squares left
  for( int i = 0; i < TOTAL_SQUARES; i++ )
  {
    if( squares[ i ].getState() == SQUARE_STATE_BLANK )
    {
      return;
    }
  }

  // If no one won, and all squares all full, we have a draw
  mCurrentState = STATE_DRAW;
}

// Prepare for new game
void Game::reset( Square squares[] )
{
  // Clear X's and O's from all squares
  for( int i = 0; i < TOTAL_SQUARES; i++ )
  {
    squares[ i ].clear();
  }

  // Alternates who goes first in each new game
  mCurrentState = mNextStartState;
  if( mNextStartState == STATE_O_TURN )
  {
    mNextStartState = STATE_X_TURN;
  }
  else
  {
    mNextStartState = STATE_O_TURN;
  }
}

// Access current state 
int Game::getState()
{
  return mCurrentState;
}
