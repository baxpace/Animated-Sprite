#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class PTexture
{
	public:
		//Initializes variables
		PTexture();

		//Deallocates memory
		~PTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL ) const;

		//Gets image dimensions
		int getWidth() const;
		int getHeight() const;

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

extern SDL_Renderer* gRenderer;
extern PTexture gSpriteSheetTexture;
extern PTexture gCupcakeTexture;