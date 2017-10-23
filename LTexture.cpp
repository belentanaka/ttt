#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "globals.h"
#include "LTexture.h"

// Initialize member variables
LTexture::LTexture()
{
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

// Free texture
LTexture::~LTexture()
{
  free();
}

// Loads image from a given file destination
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

// Destroys texture and resets member variables
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

// Renders image at a screen location (x,y)
void LTexture::render( int x, int y, SDL_Rect* clip )
{
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  if( clip != NULL )
  {
    // Render a section of image using clip Rect
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

// Access width
int LTexture::getWidth()
{
  return mWidth;
}

// Access height
int LTexture::getHeight()
{
  return mHeight;
}
