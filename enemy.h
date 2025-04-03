#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "textures.h"

class Enemy {
    public:
        Enemy(float x, float y);
        void move(float speed, float playerX, float playerY);
        void render(SDL_Renderer* renderer, PTexture& texture) const;
        float x, y;
    };

// Enemy functions
void spawnEnemy();
void moveEnemies(float speed, float playerX, float playerY);
void renderEnemies(SDL_Renderer* gRenderer, const PTexture& enemyTexture);

extern std::vector<Enemy> enemies; // Make enemies list accessible

// #endif