#pragma once

#include <SDL2/SDL.h>
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
		// void render( int x, int y, SDL_Rect* clip = NULL ) const;
		void render(int x, int y, SDL_Rect* clip = nullptr, const SDL_Rect* camera = nullptr) const;
		void renderEnemies(SDL_Renderer* renderer, PTexture& texture, const SDL_Rect& cameraView);

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

//Player texture defined in sprite_data.cpp
extern PTexture gSpriteSheetTexture; 
extern PTexture gCupcakeTexture;
extern PTexture gTileTexture;