#pragma once

#include <SDL2/SDL.h>
#include "libs/FastNoiseLite/FastNoiseLite.h"

// Constants
const int TILE_SIZE = 16;                 // Each tile is 16x16
// const int WORLD_WIDTH = 215;              // Width in tiles
// const int WORLD_HEIGHT = 96;              // Height in tiles

// Initialization 
void initNoise();

// Tile Determination 
SDL_Point getTileFromNoise(float x, float y);

// World Rendering 
void renderWorld(const SDL_Rect& cameraView);