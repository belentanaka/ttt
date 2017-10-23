#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL.h>

#include "constants.h"
#include "LTexture.h"

extern LTexture gSquareSpriteSheetTexture;
extern SDL_Rect gSquareSpriteClips[ SQUARE_SPRITE_COUNT ];

extern LTexture gGridTexture;
extern LTexture gXPromptTexture;
extern LTexture gOPromptTexture;
extern LTexture gXWinTexture;
extern LTexture gOWinTexture;
extern LTexture gDrawTexture;
extern LTexture gResetTexture;

#endif
