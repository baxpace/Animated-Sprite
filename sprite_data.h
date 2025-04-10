
//Initialize sprite sheet related variabels and functions
#pragma once

#include <SDL2/SDL.h>

extern const int WALKING_ANIMATION_FRAMES;
extern SDL_Rect gSpriteClipsDown[];
extern SDL_Rect gSpriteClipsLeft[];
extern SDL_Rect gSpriteClipsRight[];
extern SDL_Rect gSpriteClipsUp[];

void animateSpriteDown();
void animateSpriteLeft();
void animateSpriteRight();
void animateSpriteUp();