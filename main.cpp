#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>  // For sqrt and atan2
#include <vector>
#include "sprite_data.h"
#include "player.h"
#include "enemy.h"
#include <typeinfo>

// const int SCREEN_WIDTH = 1920;
// const int SCREEN_HEIGHT = 1080;
int windowWidth, windowHeight;
const int PLAYER_SPEED = 8;

int posX = windowWidth / 2;
int posY = windowHeight / 2;

// PTexture gCupcakeTexture;
const int ENEMY_SIZE = gCupcakeTexture.getWidth();  
int ENEMY_WIDTH = gCupcakeTexture.getWidth();
int ENEMY_HEIGHT = gCupcakeTexture.getHeight();


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SHOWN );

		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
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
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "HC_Humans1A_4x.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	if( !gCupcakeTexture.loadFromFile( "cupcake.png" ) )
    {
        printf( "Failed to load enemy texture!\n" );
        success = false;
    }
	else
	{
		//Set sprite clips from sprite_data.h
        animateSriteDown();
        animateSriteLeft();
        animateSriteRight();
        animateSriteUp();
	}
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Current animation frame
			int frame = 0;

			// Enemy spawn timing variables
			Uint32 lastSpawnTime = 0;
			Uint32 nextSpawnTime = 1000 + (rand() % 3000); // Random between 1000ms (1s) and 4000ms (4s)

			//Get the screen width and height
			SDL_GetWindowSize(gWindow, &windowWidth, &windowHeight);

			//Set the player position in the center of the screen
			Player player(windowWidth / 2, windowHeight / 2);
			
			//While application is running
			while (!quit)
			{
				// Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					} else if (e.type == SDL_WINDOWEVENT) {
						if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
							SDL_GetWindowSize(gWindow, &windowWidth, &windowHeight);
						}
					}

					//move player based on key input
					player.handleEvent(e);
				}
				
				// Get current time
				Uint32 currentTime = SDL_GetTicks();

				// Spawn enemy if needed
				if (currentTime - lastSpawnTime >= nextSpawnTime)
				{
					if (windowWidth == 0 || windowHeight == 0) {
						return -1;
					}

					int buffer = 100;  // Extra distance outside the screen

					// Spawn outside viewport
					int spawnX, spawnY;
					int side = rand() % 4;
					switch (side)
					{
						case 0: // Top
							spawnX = posX + (rand() % windowWidth) - (windowWidth / 2);
							spawnY = posY - (windowHeight / 2) - buffer;
							break;
						case 1: // Bottom
							spawnX = posX + (rand() % windowWidth) - (windowWidth / 2);
							spawnY = posY + (windowHeight / 2) + buffer;
							break;
						case 2: // Left
							spawnX = posX - (windowWidth / 2) - buffer;
							spawnY = posY + (rand() % windowHeight) - (windowHeight / 2);
							break;
						case 3: // Right
							spawnX = posX + (windowWidth / 2) + buffer;
							spawnY = posY + (rand() % windowHeight) - (windowHeight / 2);
							break;
					}

			        // Add new enemy to the list
					enemies.emplace_back(spawnX, spawnY);

					// Reset spawn timing
					lastSpawnTime = currentTime;
					nextSpawnTime = 1000 + (rand() % 3000); // New 1-4s interval
				}

				// Player movement logic
				int playerX = player.getX();
				int playerY = player.getY();

				// Set initial player position
				player.setPosition(playerX, playerY);

				// Get keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

				//Handle player movement & animation
				SDL_Rect* currentClip = &gSpriteClipsDown[1]; // Default sprite
				if (currentKeyStates[SDL_SCANCODE_UP]) {
					player.setY(player.getY() - PLAYER_SPEED);
					currentClip = &gSpriteClipsUp[frame / 4];
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
					player.setY(player.getY() + PLAYER_SPEED);
					currentClip = &gSpriteClipsDown[frame / 4];
				}
				else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
					player.setX(player.getX() - PLAYER_SPEED);
					currentClip = &gSpriteClipsLeft[frame / 4];
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
					player.setX(player.getX() + PLAYER_SPEED);
					currentClip = &gSpriteClipsRight[frame / 4];
				}

				// Keep player inside the window
				SDL_GetWindowSize(gWindow, &windowWidth, &windowHeight);
				if (playerX < 0) playerX = 0;
				if (playerY < 0) playerY = 0;
				if (playerX > windowWidth - 50) playerX = windowWidth - 50;
				if (playerY > windowHeight - 10) playerY = windowHeight - 10;

				// Spawns enemies on timer
				spawnEnemy();           
			
				// Move each enemy toward player (static player width/height currently being used)
				moveEnemies(enemies, playerX, playerY, 64, 128, 1.25f);

				// push enemies apart by adjusting their velocities or positions.
				separateEnemies(enemies, 32.75f);
				
				// Clear screen **only once per frame**
				SDL_SetRenderDrawColor(gRenderer, 0x87, 0x87, 0x95, 0xFF);
				SDL_RenderClear(gRenderer);

				// Render player at updated position
				player.render(gSpriteSheetTexture, currentClip);

				// Render enemies & player
				renderEnemies(gRenderer, gCupcakeTexture); 

				// Update screen (only once per frame)
				SDL_RenderPresent(gRenderer);
			
				// Cycle animation
				++frame;
				if (frame / 4 >= WALKING_ANIMATION_FRAMES)
				{
					frame = 0;
				}
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}