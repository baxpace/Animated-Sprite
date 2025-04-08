#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "textures.h"

class Enemy {
    public:
        Enemy(float x, float y);
        void move(float speed, float playerX, float playerY);
        void render(SDL_Renderer* renderer, PTexture& texture) const;
        SDL_Rect getCollisionBox() const;
        float x, y;
        int getX() const;
        int getY() const;
    };

// Enemy functions
extern void spawnEnemy();
extern void moveEnemies(std::vector<Enemy>& enemies, float playerX, float playerY, float playerWidth, float playerHeight, float speed);
extern void renderEnemies(SDL_Renderer* gRenderer, const PTexture& enemyTexture);
extern void separateEnemies(std::vector<Enemy>& enemies, float minDistance);
extern std::vector<Enemy> enemies; // Make enemies list accessible