#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>  // For sqrt and atan2
#include "entities/player/textures.h"
#include "entities/player/sprite_data.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int PLAYER_SPEED = 10;
int posX = SCREEN_WIDTH / 2;
int posY = SCREEN_HEIGHT / 2;
int enemyX = SCREEN_WIDTH / 3;
int enemyY = SCREEN_HEIGHT / 3;
const int ENEMY_SIZE = 32;  // Replace 32 with the actual width/height of your enemy texture
const int ENEMY_SPEED = 3;
int ENEMY_WIDTH = gCupcakeTexture.getWidth();
int ENEMY_HEIGHT = gCupcakeTexture.getHeight();

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

// Function to move enemy toward player
void moveEnemy(float deltaTim);

// Function to spawn enemy outside viewport
void spawnEnemy();

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
	if( !gSpriteSheetTexture.loadFromFile( "entities/player/HC_Humans1A_4x.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	if( !gCupcakeTexture.loadFromFile( "entities/enemies/cupcake/cupcake.png" ) )
    {
        printf( "Failed to load enemy texture!\n" );
        success = false;
    }
	{
		//Set sprite clips from sprite_data.h
        animateSriteDown();
        animateSriteLeft();
        animateSriteRight();
        animateSriteUp();
	}
	return success;
}

void spawnEnemy()
{
    int side = rand() % 4; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right

    switch (side)
    {
        case 0: // Spawn above the screen
            enemyX = rand() % SCREEN_WIDTH;
            enemyY = -ENEMY_SIZE;
            break;
        case 1: // Spawn below the screen
            enemyX = rand() % SCREEN_WIDTH;
            enemyY = SCREEN_HEIGHT + ENEMY_SIZE;
            break;
        case 2: // Spawn to the left
            enemyX = -ENEMY_SIZE;
            enemyY = rand() % SCREEN_HEIGHT;
            break;
        case 3: // Spawn to the right
            enemyX = SCREEN_WIDTH + ENEMY_SIZE;
            enemyY = rand() % SCREEN_HEIGHT;
            break;
    }
}

// Function to move enemy toward player
void moveEnemy(float deltaTime)
{
    // Calculate direction vector
    float dx = posX - enemyX;
    float dy = posY - enemyY;
    float length = sqrt(dx * dx + dy * dy);

    if (length != 0)
    {
        // Normalize direction
        dx /= length;
        dy /= length;

        // Move enemy towards player
        enemyX += dx * ENEMY_SPEED * deltaTime;
        enemyY += dy * ENEMY_SPEED * deltaTime;
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
					int side = rand() % 4;
					switch (side)
					{
						case 0: // Top
							enemyX = rand() % SCREEN_WIDTH;
							enemyY = -ENEMY_HEIGHT;
							break;
						case 1: // Bottom
							enemyX = rand() % SCREEN_WIDTH;
							enemyY = SCREEN_HEIGHT;
							break;
						case 2: // Left
							enemyX = -ENEMY_WIDTH;
							enemyY = rand() % SCREEN_HEIGHT;
							break;
						case 3: // Right
							enemyX = SCREEN_WIDTH;
							enemyY = rand() % SCREEN_HEIGHT;
							break;
					}
			
					// Update spawn timing
					lastSpawnTime = currentTime;
					nextSpawnTime = 1000 + (rand() % 3000); // New 1-4s interval
				}
			
				// Get keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				SDL_Rect* currentClip = &gSpriteClipsDown[1]; // Default sprite
			
				// Handle movement & animation
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
			
				// Move enemy toward player
				float enemySpeed = 1.0f; // Adjust speed as needed
				float deltaX = posX - enemyX;
				float deltaY = posY - enemyY;
				float length = sqrt(deltaX * deltaX + deltaY * deltaY);
				if (length > 0)
				{
					enemyX += (deltaX / length) * enemySpeed;
					enemyY += (deltaY / length) * enemySpeed;
				}
			
				// Clear screen **only once per frame**
				SDL_SetRenderDrawColor(gRenderer, 0x87, 0x87, 0x95, 0xFF);
				SDL_RenderClear(gRenderer);
			
				// Render enemy
				gCupcakeTexture.render(enemyX, enemyY);
			
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