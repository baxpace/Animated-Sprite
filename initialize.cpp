//Intiialize SDL, Window, media loading and a close function to dealocate resources from memory 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "initialize.h"
#include "worldgen.h"

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
		//Initilize proceedural backgrounds for worldgen.cpp
		initNoise();
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

//load the player, enemy and sprite sheet animations
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
	if (!gTileTexture.loadFromFile("simple_tiles.png")) {
		printf("Failed to load simple_tiles.png!\n");
		return false;
	}
	else
	{
		//Set sprite clips called from sprite_data.cpp
        animateSpriteDown();
        animateSpriteLeft();
        animateSpriteRight();
        animateSpriteUp();
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
	gTileTexture.free();
	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}