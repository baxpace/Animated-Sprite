#include <SDL2/SDL_image.h>
#include "textures.h"

SDL_Renderer* gRenderer = nullptr;
PTexture gSpriteSheetTexture;
PTexture gCupcakeTexture;

PTexture::PTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

PTexture::~PTexture()
{
	//Deallocate
	free();
}

bool PTexture::loadFromFile( std::string path )
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

void PTexture::free()
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

// void PTexture::render(int x, int y, SDL_Rect* clip) const { //changed from float to int if you need to troubleshoot
//     SDL_Rect renderQuad = { static_cast<int>(x), static_cast<int>(y), mWidth, mHeight };

//     // If a clip is provided, override width/height with that of the clip
//     if (clip != nullptr) {
//         renderQuad.w = clip->w;
//         renderQuad.h = clip->h;
//     }
//     SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
// }

void PTexture::render(int x, int y, SDL_Rect* clip, const SDL_Rect* camera) const {
    int renderX = x;
    int renderY = y;

    if (camera != nullptr) {
        renderX -= camera->x;
        renderY -= camera->y;
    }

    SDL_Rect renderQuad = { renderX, renderY, mWidth, mHeight };
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int PTexture::getWidth() const
{
	return mWidth;
}

int PTexture::getHeight() const
{
	return mHeight;
}