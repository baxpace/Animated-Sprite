#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>  // For sqrt and atan2
#include <vector>
// #include "textures.h"
#include "sprite_data.h"
#include "enemy.h"
#include <typeinfo>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int PLAYER_SPEED = 10;
const int ENEMY_SIZE = 32;  // Replace 32 with the actual width/height of your enemy texture
int posX = SCREEN_WIDTH / 2;
int posY = SCREEN_HEIGHT / 2;

// PTexture gCupcakeTexture;
int ENEMY_WIDTH = gCupcakeTexture.getWidth();
int ENEMY_HEIGHT = gCupcakeTexture.getHeight();


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

// Move enemy towards player function
// void moveEnemies(float speed);

// List to track all active enemies
// std::vector<Enemy> enemies;

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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

// Move enemy towards player function
void moveEnemies(float speed)
{
	for (auto& enemy : enemies)
	{
		float deltaX = posX - enemy.x;
		float deltaY = posY - enemy.y;
		float length = sqrt(deltaX * deltaX + deltaY * deltaY);

		if (length > 1.0f) // Prevent jittering when too close
		{
			enemy.x += (deltaX / length) * speed;
			enemy.y += (deltaY / length) * speed;
		}
	}
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

			//While application is running
			while (!quit)
			{
				// Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
			
				// Get current time
				Uint32 currentTime = SDL_GetTicks();
			
				// Spawn enemy if needed
				if (currentTime - lastSpawnTime >= nextSpawnTime)
				{
					// Spawn outside viewport
					int spawnX, spawnY;
					int side = rand() % 4;

					switch (side)
					{
						case 0: // Top
							spawnX = rand() % SCREEN_WIDTH;
							spawnY = -ENEMY_HEIGHT;
							break;
						case 1: // Bottom
							spawnX = rand() % SCREEN_WIDTH;
							spawnY = SCREEN_HEIGHT;
							break;
						case 2: // Left
							spawnX = -ENEMY_WIDTH;
							spawnY = rand() % SCREEN_HEIGHT;
							break;
						case 3: // Right
							spawnX = SCREEN_WIDTH;
							spawnY = rand() % SCREEN_HEIGHT;
							break;
					}

			        // Add new enemy to the list
					enemies.emplace_back(spawnX, spawnY);

					// Reset spawn timing
					lastSpawnTime = currentTime;
					nextSpawnTime = 1000 + (rand() % 3000); // New 1-4s interval
				}
			
				// Get keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				SDL_Rect* currentClip = &gSpriteClipsDown[1]; // Default sprite
			
				// Handle player movement & animation
				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					posY -= PLAYER_SPEED;
					currentClip = &gSpriteClipsUp[frame / 4];
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					posY += PLAYER_SPEED;
					currentClip = &gSpriteClipsDown[frame / 4];
				}
				else if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					posX -= PLAYER_SPEED;
					currentClip = &gSpriteClipsLeft[frame / 4];
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					posX += PLAYER_SPEED;
					currentClip = &gSpriteClipsRight[frame / 4];
				}

				// Spawns enemies on timer
				spawnEnemy();           
			
				// Move each enemy toward player
				moveEnemies(1.5f, posX, posY);; // Adjust speed as needed
				
				// Clear screen **only once per frame**
				SDL_SetRenderDrawColor(gRenderer, 0x87, 0x87, 0x95, 0xFF);
				SDL_RenderClear(gRenderer);
			
				// Render enemies & player
				renderEnemies(gRenderer, gCupcakeTexture); 

				// std::cout << "Type of gCupcakeTexture: " << typeid(gCupcakeTexture).name() << std::endl;

				gSpriteSheetTexture.render(posX, posY, currentClip);
			
				// Render player at updated position
				gSpriteSheetTexture.render(posX, posY, currentClip);
			
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