#include "enemy.h"
#include <cstdlib> // For rand()
#include <cmath>   // For sqrt


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int ENEMY_WIDTH = 32;    // Adjust based on your sprite size
const int ENEMY_HEIGHT = 32;    // Adjust based on your sprite size

Uint32 lastSpawnTime = 0;
Uint32 nextSpawnTime = 1000 + (rand() % 3000);
std::vector<Enemy> enemies;

// Constructor
Enemy::Enemy(float startX, float startY) : x(startX), y(startY) {}

// Spawn enemies at random screen edges
void spawnEnemy()
{
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastSpawnTime < nextSpawnTime)
        return; // Skip if timer hasn't expired

    int spawnX, spawnY;
    int side = rand() % 4;

    switch (side)
    {
        case 0: spawnX = rand() % SCREEN_WIDTH; spawnY = -ENEMY_HEIGHT; break; // Top
        case 1: spawnX = rand() % SCREEN_WIDTH; spawnY = SCREEN_HEIGHT; break; // Bottom
        case 2: spawnX = -ENEMY_WIDTH; spawnY = rand() % SCREEN_HEIGHT; break; // Left
        case 3: spawnX = SCREEN_WIDTH; spawnY = rand() % SCREEN_HEIGHT; break; // Right
    }

    enemies.emplace_back(spawnX, spawnY);
    lastSpawnTime = currentTime;
    nextSpawnTime = 1000 + (rand() % 3000); // Reset spawn timer
}

void moveEnemies(float speed, float playerX, float playerY)
{
    for (auto& enemy : enemies)
    {
        float deltaX = playerX - enemy.x;
        float deltaY = playerY - enemy.y;
        float length = sqrt(deltaX * deltaX + deltaY * deltaY);

        if (length > 1.0f)
        {
            enemy.x += (deltaX / length) * speed;
            enemy.y += (deltaY / length) * speed;
        }
    }
}

// Render enemies
void renderEnemies(SDL_Renderer* renderer, const PTexture& enemyTexture)
{
    for (const auto& enemy : enemies)
    {
        enemyTexture.render(enemy.x, enemy.y);
    }
}