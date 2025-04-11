#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "textures.h"

class Enemy {
    public:
        Enemy(float x, float y, int damage = 2);
        float x, y;
        void move(float speed, float playerX, float playerY);
        // void render(SDL_Renderer* renderer, PTexture& texture) const;
        void render(SDL_Renderer* renderer, PTexture& texture, const SDL_Rect& cameraView) const;
        SDL_Rect getCollisionBox() const;
        SDL_Color getParticleColor() const;
        int getX() const;
        int getY() const;
        int getDamage() const;
        SDL_Color getRandomizedColor(const SDL_Color& base) const;
    private:
        int damage;
        std::vector<SDL_Color> cupcakeColors = {
            {245, 222, 179, 255}, // cake beige
            {255, 255, 255, 255}, // icing white
            {255, 50, 50, 255},   // cherry
            {30, 30, 30, 255},    // eyes
            {173, 216, 230, 255}, // sprinkle blue
            {200, 160, 255, 255}  // sprinkle purple
        };
    };

extern void spawnEnemy();
extern std::vector<Enemy> enemies; // Places spawed enemies in a list
extern void moveEnemies(std::vector<Enemy>& enemies, float playerX, float playerY, float playerWidth, float playerHeight, float speed);
extern void separateEnemies(std::vector<Enemy>& enemies, float minDistance);
extern void renderEnemies(SDL_Renderer* renderer, PTexture& texture, const SDL_Rect& cameraView);


