#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "textures.h"
#include "sprite_data.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
