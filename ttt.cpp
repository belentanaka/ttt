#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TOTAL_ROWS = 3;
const int TOTAL_COLS = 3;
const int TOTAL_SQUARES = TOTAL_ROWS * TOTAL_COLS;
const int WIN_NUMBER = 3;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

class LTexture
{
  public:
  LTexture();
  ~LTexture();
  bool loadFromFile( std::string path );
  void free();
  void render( int x, int y, SDL_Rect* clip = NULL );
  int getWidth();
  int getHeight();

  private:
  SDL_Texture* mTexture;
  int mWidth;
  int mHeight;
};

LTexture::LTexture()
{
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture()
{
  free();
}

bool LTexture::loadFromFile( std::string path )
{
  free();

  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );
    
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
    {
      printf( "Failed to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }

    SDL_FreeSurface( loadedSurface );
  }

  mTexture = newTexture;
  return mTexture != NULL;
}

void LTexture::free()
{
  if( mTexture != NULL )
  {
    SDL_DestroyTexture( mTexture );
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  if( clip != NULL )
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
  return mWidth;
}

int LTexture::getHeight()
{
  return mHeight;
}

LTexture gSquareSpriteSheet;
LTexture gGrid;
LTexture gXPrompt;
LTexture gOPrompt;
LTexture gXWin;
LTexture gOWin;
LTexture gDraw;
LTexture gReset;

enum LSquareState
{
  SQUARE_STATE_BLANK,
  SQUARE_STATE_X,
  SQUARE_STATE_O
};

enum LSquareSprite
{
  SQUARE_SPRITE_BLANK = 0,
  SQUARE_SPRITE_X = 1,
  SQUARE_SPRITE_O = 5,
  SQUARE_SPRITE_COUNT = 9
};

SDL_Rect gSquareSpriteClips[ SQUARE_SPRITE_COUNT ];

enum GameStateFlag
{
  STATE_X_TURN,
  STATE_O_TURN,
  STATE_X_WIN,
  STATE_O_WIN,
  STATE_DRAW,
};

class LSquare
{
  public:
  static const int SQUARE_WIDTH = 100;
  static const int SQUARE_HEIGHT = 100;

  LSquare();
  void setPosition( int x, int y );
  bool handleEvent( SDL_Event* e, int gameState );
  void clear();
  void render();
  int getState();

  private:
  SDL_Point mPosition;
  LSquareState mCurrentState;
  LSquareSprite mCurrentSprite;
  Uint32 mInitialTicks;
  Uint32 mFinalTicks;
};

LSquare::LSquare()
{
  mPosition.x = 0;
  mPosition.y = 0;

  mCurrentState = SQUARE_STATE_BLANK;
  mCurrentSprite = SQUARE_SPRITE_BLANK;
  mInitialTicks = SDL_GetTicks();
  mFinalTicks = mInitialTicks;
}

void LSquare::setPosition( int x, int y )
{
  mPosition.x = x;
  mPosition.y = y;
}

bool LSquare::handleEvent( SDL_Event* e, int gameState )
{
  bool turnComplete = false;

  if( ( mCurrentState == SQUARE_STATE_BLANK ) && ( e->type == SDL_MOUSEBUTTONUP ) )
  {
    int x, y;
    SDL_GetMouseState( &x, &y );

    if( ( x > mPosition.x ) && ( x < mPosition.x + SQUARE_WIDTH ) && ( y > mPosition.y ) && ( y < mPosition.y + SQUARE_HEIGHT ) )
    {
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

  return turnComplete;
}

void LSquare::clear()
{
  mCurrentState = SQUARE_STATE_BLANK;
  mCurrentSprite = SQUARE_SPRITE_BLANK;
  mInitialTicks = SDL_GetTicks();
  mFinalTicks = mInitialTicks;
}

void LSquare::render()
{
  if( mCurrentState == SQUARE_STATE_BLANK )
  {  
    gSquareSpriteSheet.render( mPosition.x, mPosition.y, &gSquareSpriteClips[ mCurrentSprite ] );
  }
  else if( mCurrentState == SQUARE_STATE_X )
  {
    gSquareSpriteSheet.render( mPosition.x, mPosition.y, &gSquareSpriteClips[ mCurrentSprite + ( ( mFinalTicks - mInitialTicks ) / 100 ) ] );
    
    mFinalTicks = SDL_GetTicks();
    if( ( mFinalTicks - mInitialTicks ) / 100 >= 4 )
    {
      mInitialTicks = mFinalTicks;
    }
  }
  else if( mCurrentState == SQUARE_STATE_O )
  {
    gSquareSpriteSheet.render( mPosition.x, mPosition.y, &gSquareSpriteClips[ mCurrentSprite + ( ( mFinalTicks - mInitialTicks ) / 100 ) ] );

    mFinalTicks = SDL_GetTicks();
    if( ( mFinalTicks - mInitialTicks ) / 100 >= 4 )
    {
      mInitialTicks = mFinalTicks;
    }
  }
}

int LSquare::getState()
{
  return mCurrentState;
}

class GameState
{
  public:
  GameState();
  int getState();
  void progress( LSquare squares[] );
  void completeTurn();
  void reset( LSquare squares[] );

  private:
  GameStateFlag mCurrentState;
  GameStateFlag mNextStartState;
};

GameState::GameState()
{
  mCurrentState = STATE_X_TURN;
  mNextStartState = STATE_O_TURN;
}

int GameState::getState()
{
  return mCurrentState;
}

void GameState::progress( LSquare squares[] )
{
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

    mCurrentState = STATE_O_TURN;
  }
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

    mCurrentState = STATE_X_TURN;
  }

  for( int i = 0; i < TOTAL_SQUARES; i++ )
  {
    if( squares[ i ].getState() == SQUARE_STATE_BLANK )
    {
      return;
    }
  }

  mCurrentState = STATE_DRAW;
}

void GameState::reset( LSquare squares[] )
{
  for( int i = 0; i < TOTAL_SQUARES; i++ )
  {
    squares[ i ].clear();
  }

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

bool loadMedia()
{
  bool success = true;

  if( !gSquareSpriteSheet.loadFromFile( "media/square.png" ) )
  {
    printf( "Failed to load square sprite sheet texture!\n" );
    success = false;
  }
  else
  {
    gSquareSpriteClips[ 0 ].x = 0;
    gSquareSpriteClips[ 0 ].y = 0;
    gSquareSpriteClips[ 0 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 0 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 1 ].x = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 1 ].y = 0;
    gSquareSpriteClips[ 1 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 1 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 2 ].x = 2 * LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 2 ].y = 0;
    gSquareSpriteClips[ 2 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 2 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 3 ].x = 3 * LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 3 ].y = 0;
    gSquareSpriteClips[ 3 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 3 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 4 ].x = 4 * LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 4 ].y = 0;
    gSquareSpriteClips[ 4 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 4 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 5 ].x = 0;
    gSquareSpriteClips[ 5 ].y = LSquare::SQUARE_HEIGHT;
    gSquareSpriteClips[ 5 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 5 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 6 ].x = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 6 ].y = LSquare::SQUARE_HEIGHT;
    gSquareSpriteClips[ 6 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 6 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 7 ].x = 2 * LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 7 ].y = LSquare::SQUARE_HEIGHT;
    gSquareSpriteClips[ 7 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 7 ].h = LSquare::SQUARE_HEIGHT;

    gSquareSpriteClips[ 8 ].x = 3 * LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 8 ].y = LSquare::SQUARE_HEIGHT;
    gSquareSpriteClips[ 8 ].w = LSquare::SQUARE_WIDTH;
    gSquareSpriteClips[ 8 ].h = LSquare::SQUARE_HEIGHT;
  }

  if( !gGrid.loadFromFile( "media/grid.png" ) )
  {
    printf( "Failed to load grid texture!\n" );
    success = false;
  }

  if( !gXPrompt.loadFromFile( "media/xprompt.png" ) )
  {
    printf( "Failed to load X prompt texture!\n" );
    success = false;
  }

  if( !gOPrompt.loadFromFile( "media/oprompt.png" ) )
  {
    printf( "Failed to load O prompt texture!\n" );
    success = false;
  }

  if( !gXWin.loadFromFile( "media/xwin.png" ) )
  {
    printf( "Failed to load X win texture!\n" );
    success = false;
  }

  if( !gOWin.loadFromFile( "media/owin.png" ) )
  {
    printf( "Failed to load O win texture!\n" );
    success = false;
  }

  if( !gDraw.loadFromFile( "media/draw.png" ) )
  {
    printf( "Failed to load draw texture!\n" );
    success = false;
  }

  if( !gReset.loadFromFile( "media/reset.png" ) )
  {
    printf( "Failed to load reset texture!\n" );
    success = false;
  }

  return success;
}

void close()
{
  gSquareSpriteSheet.free();
  gGrid.free();
  gXPrompt.free();
  gOPrompt.free();
  gXWin.free();
  gOWin.free();
  gDraw.free();
  gReset.free();

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

    LSquare squares[ TOTAL_SQUARES ];
    int xOffset = 0;
    int yOffset = 0;
    for( int i = 0; i < TOTAL_SQUARES; i++ )
    {
      squares[ i ].setPosition( 173 + xOffset, 100 + yOffset );

      xOffset += LSquare::SQUARE_WIDTH;
      if( xOffset >= TOTAL_COLS * LSquare::SQUARE_WIDTH )
      {
	xOffset = 0;
	yOffset += LSquare::SQUARE_HEIGHT;
      }
    }
   
    while( !quit )
    {
      bool turnComplete = false;

      while( SDL_PollEvent( &e ) != 0 )
      {
	if( e.type == SDL_QUIT )
	{
	  quit = true;
	}
	else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r )
	{
	  int currentState = gameState.getState();
	  if( currentState == STATE_X_WIN || currentState == STATE_O_WIN || currentState == STATE_DRAW )
	  {
	    gameState.reset( squares );
	  }
	}

	for( int i = 0; i < TOTAL_SQUARES; i++ )
	{
	  if( squares[ i ].handleEvent( &e, gameState.getState() ) )
	  {
	    turnComplete = true;
	    break;
	  }
	}
      }

      if( turnComplete )
      {
        gameState.progress( squares );
      }

      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      SDL_RenderClear( gRenderer );
      
      gGrid.render( 0, 0 );
      
      for( int i = 0; i < TOTAL_SQUARES; i++ )
      {
	squares[ i ].render();
      }

      switch( gameState.getState() )
      {
        case STATE_X_TURN:
	  gXPrompt.render( 0, 0 );
	  break;

        case STATE_O_TURN:
	  gOPrompt.render( 0, 0 );
	  break;

        case STATE_X_WIN:
	  gXWin.render( 0, 0 );
	  gReset.render( 0, SCREEN_HEIGHT - gReset.getHeight() );
	  break;

        case STATE_O_WIN:
	  gOWin.render( 0, 0 );
	  gReset.render( 0, SCREEN_HEIGHT - gReset.getHeight() );
	  break;

        case STATE_DRAW:
	  gDraw.render( 0, 0 );
	  gReset.render( 0, SCREEN_HEIGHT - gReset.getHeight() );
	  break;
      }

      SDL_RenderPresent( gRenderer );
    }
  }

  close();
  return 0;
}
