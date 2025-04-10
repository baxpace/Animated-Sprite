#include "spawner.h"
#include <cstdlib>

Spawner::Spawner(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight), lastSpawnTime(0), nextSpawnTime(1000 + rand() % 3000), buffer(100) {}

void Spawner::setNextSpawnTime(int minTime, int maxTime) {
    nextSpawnTime = minTime + (rand() % (maxTime - minTime));
}

void Spawner::update(Uint32 currentTime, std::vector<Enemy>& enemies) {
    // Spawn enemy if needed
    if (currentTime - lastSpawnTime >= nextSpawnTime) {
        if (windowWidth == 0 || windowHeight == 0) {
            return;
        }

        // Call the function to spawn a new enemy
        spawnEnemy(enemies);

        // Reset spawn timing
        lastSpawnTime = currentTime;
        nextSpawnTime = 1000 + (rand() % 3000); // New 1-4s interval
    }
}

void Spawner::spawnEnemy(std::vector<Enemy>& enemies) {
    // Spawn outside viewport
    int spawnX, spawnY;
    int side = rand() % 4;
    switch (side) {
        case 0: // Top
            spawnX = rand() % windowWidth;
            spawnY = -buffer;  // Top side
            break;
        case 1: // Bottom
            spawnX = rand() % windowWidth;
            spawnY = windowHeight + buffer; // Bottom side
            break;
        case 2: // Left
            spawnX = -buffer;  // Left side
            spawnY = rand() % windowHeight;
            break;
        case 3: // Right
            spawnX = windowWidth + buffer;  // Right side
            spawnY = rand() % windowHeight;
            break;
    }

    // Create and add the enemy to the vector
    enemies.emplace_back(spawnX, spawnY);

    // Optionally, you could introduce more spawn logic here based on the enemy type
}