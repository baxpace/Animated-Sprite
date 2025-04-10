#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include "enemy.h"
#include "collision.h"
#include "initialize.h"
#include "player.h"
#include "spawner.h"
#include "particle.h"
#include "sprite_data.h"
#include "worldgen.h"

int windowWidth = 1920;
int windowHeight = 1080;
int posX = windowWidth / 2;
int posY = windowHeight / 2;
int ENEMY_WIDTH = gCupcakeTexture.getWidth();
int ENEMY_HEIGHT = gCupcakeTexture.getHeight();

Uint32 lastTicks = SDL_GetTicks(); 
Spawner spawner(windowWidth, windowHeight);

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
			bool quit = false;										 	// Main loop flag
			SDL_Event e;												// Event handler
			int frame = 0;											 	// Current animation frame
			Uint32 lastSpawnTime = 0;								 	// Enemy spawn timing variables
			Uint32 nextSpawnTime = 1000 + (rand() % 3000); 			 	// Random between 1000ms (1s) and 4000ms (4s)
			SDL_GetWindowSize(gWindow, &windowWidth, &windowHeight); 	// Get the screen width and height
			Player player(windowWidth / 2, windowHeight / 2);		 	// Set the player position in the center of the screen
			Uint32 currentTicks = SDL_GetTicks();						// Get current time
			float deltaTime = (currentTicks - lastTicks) / 1000.0f; 	// in seconds
  			lastTicks = currentTicks;
			std::vector<Particle> particles;							// a vector for spawned particles

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
				}

				// Player movement logic
				int playerX = player.getX();
				int playerY = player.getY();

				// Set initial player position
				player.setPosition(playerX, playerY);

				// Get keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
				Uint32 currentTime = SDL_GetTicks();    
				spawner.update(currentTime, enemies);
			
				// Move each enemy toward player (static player width/height currently being used)
				moveEnemies(enemies, playerX, playerY, 64, 128, 1.25f);

				for (auto it = enemies.begin(); it != enemies.end(); ) {
					if (checkCollision(player.getCollisionBox(), it->getCollisionBox())) {
						player.takeDamage(it->getDamage());
				
						// Spawn explosion particles when collision detected
						for (int i = 0; i < 100; ++i) {
							// Use the getRandomizedColor function from Enemy (or Cupcake)
							SDL_Color baseColor = it->getParticleColor();  // Assuming this function exists to return a base color
							SDL_Color variedColor = it->getRandomizedColor(baseColor);
							particles.emplace_back(it->getX(), it->getY(), variedColor);
						}
				
						it = enemies.erase(it);
					} else {
						++it;
					}
				}

				// push enemies apart by adjusting their velocities or positions.
				separateEnemies(enemies, 32.75f);
				
				// Clear screen **only once per frame**
				SDL_SetRenderDrawColor(gRenderer, 0x87, 0x87, 0x95, 0xFF);

				// Clear the current renderer
				SDL_RenderClear(gRenderer);
				renderWorld();
				SDL_Rect* currentClip = player.getCurrentAnimationClip(frame);

				// Render player at updated position
				player.handleInput(currentKeyStates);
				player.move(windowWidth, windowHeight);
				player.render(gRenderer, gSpriteSheetTexture, currentClip);

				// Render health bar border
				SDL_Rect healthBorder = player.getHealthBarBorderRect();
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Black border
				SDL_RenderFillRect(gRenderer, &healthBorder); // Render the border behind the healthbar

				// Render health bar on top of border
				SDL_Rect healthBar = player.getHealthBarRect();
				if (player.getIsFlashing()) {
					SDL_SetRenderDrawColor(gRenderer, 255, 100, 100, 255); // Light red flash
				} else {
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Normal red
				}

				// Render and fill in rect with colour
				SDL_RenderFillRect(gRenderer, &healthBar);

				//set flash true or false
				player.updateFlash(); 

				// Render enemies & player
				renderEnemies(gRenderer, gCupcakeTexture);
				
				// Spawn particles and eleminate if isAlive reports false (age < lifeTime)
				for (auto it = particles.begin(); it != particles.end(); ) {
					it->update(deltaTime);
					if (!it->isAlive()) {
						it = particles.erase(it);
					} else {
						++it;
					}
				}
				//render particle effect
				for (const auto& p : particles) {
					p.render(gRenderer);
				} 

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
	//smoke a bowl
	return 0;
}