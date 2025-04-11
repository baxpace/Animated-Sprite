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
#include "camera.h"

int windowWidth = 1920;
int windowHeight = 1080;
int posX = windowWidth / 2;
int posY = windowHeight / 2;
int ENEMY_WIDTH = gCupcakeTexture.getWidth();
int ENEMY_HEIGHT = gCupcakeTexture.getHeight();

Uint32 lastTicks = SDL_GetTicks(); 
Spawner spawner(windowWidth, windowHeight);
Camera camera(windowWidth, windowHeight);

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
			Player player(0, 0);		 	// Set the player position in the center of the screen
			Uint32 currentTicks = SDL_GetTicks();						// Get current time
			float deltaTime = (currentTicks - lastTicks) / 1000.0f; 	// in seconds
  			lastTicks = currentTicks;
			particlePool.reserve(MAX_PARTICLES);
			// Set initial player position
			player.setPosition(windowWidth, windowHeight);

			// While application is running
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

				// Get keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
				Uint32 currentTime = SDL_GetTicks();    
				spawner.update(currentTime, enemies);
			
				for (auto it = enemies.begin(); it != enemies.end(); ) {
					if (checkCollision(player.getCollisionBox(), it->getCollisionBox())) {
						player.takeDamage(it->getDamage());
						
						for (int i = 0; i < 100; ++i) {
							SDL_Color baseColor = it->getParticleColor();
							SDL_Color variedColor = it->getRandomizedColor(baseColor);
							spawnParticle(it->getX(), it->getY(), variedColor);
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
				std::cout << "Rendering player at: " << &player << " Pos: " << player.getX() << ", " << player.getY() << "\n";

				// Render player at updated position
				// Handle input and movement
				player.handleInput(currentKeyStates);
				player.move();
								
				// Move each enemy toward player (static player width/height currently being used)
				moveEnemies(enemies, player.getX(), player.getY(), 64, 128, 1.25f);
				std::cout << "Enemy moving towards: " << player.getX() << ", " << player.getY() << "\n";

				camera.centerOn(player.getX() + 32, player.getY() + 64); 

				// Render background with camera offset
				renderWorld(camera.getView());
				SDL_Rect view = camera.getView();
				std::cout << "Camera X: " << view.x << " Y: " << view.y << std::endl;

				// Render player (texture + health bar) with camera offset
				SDL_Rect* currentClip = player.getCurrentAnimationClip(frame);
				player.render(gRenderer, gSpriteSheetTexture, currentClip, camera.getView());

				std::cout << "Player Pos: " << player.getX() << ", " << player.getY() << "\n";
				std::cout << "Camera: " << camera.getView().x << ", " << camera.getView().y << "\n";

				//set flash true or false
				player.updateFlash(); 

				// Render enemies & player
				// renderEnemies(gRenderer, gCupcakeTexture);
				
				renderEnemies(gRenderer, gCupcakeTexture, camera.getView());
				
				// Spawn particles and eliminate if isAlive reports false (age < lifeTime)
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
				
				updateAndRenderParticles(gRenderer, deltaTime);

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