#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "textures.h"

class Enemy {
    public:
        Enemy(float x, float y, int damage = 2);
        float x, y;
        void move(float speed, float playerX, float playerY);
        void render(SDL_Renderer* renderer, PTexture& texture) const;
        SDL_Rect getCollisionBox() const;
        SDL_Color getParticleColor() const;
        int getX() const;
        int getY() const;
        int getDamage() const;
    private:
        int damage;
    };

extern void spawnEnemy();
extern std::vector<Enemy> enemies; // Places spawed enemies in a list
extern void moveEnemies(std::vector<Enemy>& enemies, float playerX, float playerY, float playerWidth, float playerHeight, float speed);
extern void renderEnemies(SDL_Renderer* gRenderer, const PTexture& enemyTexture);
extern void separateEnemies(std::vector<Enemy>& enemies, float minDistance);
