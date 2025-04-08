
#pragma once
#include <SDL2/SDL.h>

extern const int WALKING_ANIMATION_FRAMES;

extern SDL_Rect gSpriteClipsDown[];
extern SDL_Rect gSpriteClipsLeft[];
extern SDL_Rect gSpriteClipsRight[];
extern SDL_Rect gSpriteClipsUp[];

void animateSriteDown();
void animateSriteLeft();
void animateSriteRight();
void animateSriteUp();


