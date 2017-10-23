#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "constants.h"
#include "globals.h"
#include "LTexture.h"
#include "textures.h"
#include "Square.h"
#include "GameState.h"

// Intialize SDL subsystems 
bool init()
{
  bool success = true;
  
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    gWindow = SDL_CreateWindow( "TTT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
      printf( "SDL could not create window! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL )
      {
	printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
	success = false;
      }
      else
      {
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
	  printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
	  success = false;
	}
      }
    }
  }

  return success;
}

// Load images from file destinations
bool loadMedia()
{
  bool success = true;

  // Square sprite sheet used for X's and O's
  if( !gSquareSpriteSheetTexture.loadFromFile( "media/square.png" ) )
  {
    printf( "Failed to load square sprite sheet texture!\n" );
    success = false;
  }
  else
  {
    // Create clip Rects for each animation sprite
    gSquareSpriteClips[ 0 ].x = 0;
    gSquareSpriteClips[ 0 ].y = 0;
    gSquareSpriteClips[ 0 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 0 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 1 ].x = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 1 ].y = 0;
    gSquareSpriteClips[ 1 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 1 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 2 ].x = 2 * Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 2 ].y = 0;
    gSquareSpriteClips[ 2 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 2 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 3 ].x = 3 * Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 3 ].y = 0;
    gSquareSpriteClips[ 3 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 3 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 4 ].x = 4 * Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 4 ].y = 0;
    gSquareSpriteClips[ 4 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 4 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 5 ].x = 0;
    gSquareSpriteClips[ 5 ].y = Square::SQUARE_HEIGHT;
    gSquareSpriteClips[ 5 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 5 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 6 ].x = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 6 ].y = Square::SQUARE_HEIGHT;
    gSquareSpriteClips[ 6 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 6 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 7 ].x = 2 * Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 7 ].y = Square::SQUARE_HEIGHT;
    gSquareSpriteClips[ 7 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 7 ].h = Square::SQUARE_HEIGHT;

    gSquareSpriteClips[ 8 ].x = 3 * Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 8 ].y = Square::SQUARE_HEIGHT;
    gSquareSpriteClips[ 8 ].w = Square::SQUARE_WIDTH;
    gSquareSpriteClips[ 8 ].h = Square::SQUARE_HEIGHT;
  }

  if( !gGridTexture.loadFromFile( "media/grid.png" ) )
  {
    printf( "Failed to load grid texture!\n" );
    success = false;
  }

  if( !gXPromptTexture.loadFromFile( "media/xprompt.png" ) )
  {
    printf( "Failed to load X prompt texture!\n" );
    success = false;
  }

  if( !gOPromptTexture.loadFromFile( "media/oprompt.png" ) )
  {
    printf( "Failed to load O prompt texture!\n" );
    success = false;
  }

  if( !gXWinTexture.loadFromFile( "media/xwin.png" ) )
  {
    printf( "Failed to load X win texture!\n" );
    success = false;
  }

  if( !gOWinTexture.loadFromFile( "media/owin.png" ) )
  {
    printf( "Failed to load O win texture!\n" );
    success = false;
  }

  if( !gDrawTexture.loadFromFile( "media/draw.png" ) )
  {
    printf( "Failed to load draw texture!\n" );
    success = false;
  }

  if( !gResetTexture.loadFromFile( "media/reset.png" ) )
  {
    printf( "Failed to load reset texture!\n" );
    success = false;
  }

  return success;
}

// Destroy textures and SDL objects, and close SDL subsystems
void close()
{
  gSquareSpriteSheetTexture.free();
  gGridTexture.free();
  gXPromptTexture.free();
  gOPromptTexture.free();
  gXWinTexture.free();
  gOWinTexture.free();
  gDrawTexture.free();
  gResetTexture.free();

  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;

  IMG_Quit();
  SDL_Quit();
}

int main()
{
  if( !init() )
  {
    printf( "Failed to initialize!\n" );
  }
  else if( !loadMedia() )
  {
    printf( "Failed to load media!\n" );
  }
  else
  {
    bool quit = false;
    SDL_Event e;
    GameState gameState;

    Square squares[ TOTAL_SQUARES ];
    int xOffset = 0;
    int yOffset = 0;

    // Place Squares on grid
    for( int i = 0; i < TOTAL_SQUARES; i++ )
    {
      squares[ i ].setPosition( 173 + xOffset, 100 + yOffset );

      xOffset += Square::SQUARE_WIDTH;
      if( xOffset >= TOTAL_COLS * Square::SQUARE_WIDTH )
      {
	xOffset = 0;
	yOffset += Square::SQUARE_HEIGHT;
      }
    }
   
    while( !quit )
    {
      bool turnComplete = false;

      while( SDL_PollEvent( &e ) != 0 )
      {
	// Detect if user quits
	if( e.type == SDL_QUIT )
	{
	  quit = true;
	}
	// Detect if user pressed 'r'
	else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r )
	{
	  int currentState = gameState.getState();
	  if( currentState == STATE_X_WIN || currentState == STATE_O_WIN || currentState == STATE_DRAW )
	  {
	    // Prepare new game if current game is over
	    gameState.reset( squares );
	  }
	}

	// Check if current player clicked on any Square
	for( int i = 0; i < TOTAL_SQUARES; i++ )
	{
	  if( squares[ i ].handleEvent( &e, gameState.getState() ) )
	  {
	    // If so, player's turn is over
	    turnComplete = true;
	    break;
	  }
	}
      }

      // If turn is complete, progress game state based on Squares
      if( turnComplete )
      {
        gameState.progress( squares );
      }

      // Prepare for rendering
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      SDL_RenderClear( gRenderer );
      
      // Draw grid
      gGridTexture.render( 0, 0 );
      
      // Draw X's and O's at Squares
      for( int i = 0; i < TOTAL_SQUARES; i++ )
      {
	squares[ i ].render();
      }

      // Draw the appropriate prompts
      switch( gameState.getState() )
      {
        case STATE_X_TURN:
	  gXPromptTexture.render( 0, 0 );
	  break;

        case STATE_O_TURN:
	  gOPromptTexture.render( 0, 0 );
	  break;

        case STATE_X_WIN:
	  gXWinTexture.render( 0, 0 );
	  gResetTexture.render( 0, SCREEN_HEIGHT - gResetTexture.getHeight() );
	  break;

        case STATE_O_WIN:
	  gOWinTexture.render( 0, 0 );
	  gResetTexture.render( 0, SCREEN_HEIGHT - gResetTexture.getHeight() );
	  break;

        case STATE_DRAW:
	  gDrawTexture.render( 0, 0 );
	  gResetTexture.render( 0, SCREEN_HEIGHT - gResetTexture.getHeight() );
	  break;
      }

      SDL_RenderPresent( gRenderer );
    }
  }

  close();
  return 0;
}
