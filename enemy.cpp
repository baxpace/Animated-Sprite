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

// Render enemies
void renderEnemies(SDL_Renderer* renderer, const PTexture& enemyTexture)
{
    for (const auto& enemy : enemies)
    {
        enemyTexture.render(enemy.x, enemy.y);
    }
}

void moveEnemies(std::vector<Enemy>& enemies, float posX, float posY, float playerWidth, float playerHeight, float speed) {
    float targetX = posX + playerWidth / 2.0f;  // Adjust target to center
    float targetY = posY + playerHeight / 2.0f;

    for (auto& enemy : enemies) {
        float dx = targetX - enemy.x;
        float dy = targetY - enemy.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance > 0) {  // Avoid division by zero
            float moveX = (dx / distance) * speed;
            float moveY = (dy / distance) * speed;

            enemy.x += moveX;
            enemy.y += moveY;
        }
    }
}

void separateEnemies(std::vector<Enemy>& enemies, float minDistance) {
    for (size_t i = 0; i < enemies.size(); i++) {
        for (size_t j = i + 1; j < enemies.size(); j++) {
            float dx = enemies[j].x - enemies[i].x;
            float dy = enemies[j].y - enemies[i].y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < minDistance && distance > 0) { // Avoid division by zero
                float overlap = (minDistance - distance) / 2.0f;

                // Normalize displacement vector
                float nx = dx / distance;
                float ny = dy / distance;

                // Push enemies apart
                enemies[i].x -= nx * overlap;
                enemies[i].y -= ny * overlap;
                enemies[j].x += nx * overlap;
                enemies[j].y += ny * overlap;
            }
        }
    }
}