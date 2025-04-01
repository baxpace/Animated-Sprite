#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "entities/player/player_texture.h"

class CupcakeTexture
{
	public:
		//Initializes variables
		CupcakeTexture();

		//Deallocates memory
		~CupcakeTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		// void render();

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
        //The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
};

SDL_Renderer* gRenderer = NULL;
CupcakeTexture cupcakeTexture;

CupcakeTexture::CupcakeTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

CupcakeTexture::~CupcakeTexture()
{
	//Deallocate
	free();
}

bool CupcakeTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void CupcakeTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

// void CupcakeTexture::render()
// {
//     //Show the dot
// 	cupcakeTexture.render( mPosX, mPosY );
// }


int CupcakeTexture::getWidth()
{
	return mWidth;
}

int CupcakeTexture::getHeight()
{
	return mHeight;
}