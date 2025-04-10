#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>
#include "enemy.h" // or "cupcake.h" for specific enemy classes

class Spawner {
public:
    Spawner(int windowWidth, int windowHeight);
    void update(Uint32 currentTime, std::vector<Enemy>& enemies);
    void setNextSpawnTime(int minTime, int maxTime); // To allow variation in spawn intervals

private:
    int windowWidth, windowHeight;
    Uint32 lastSpawnTime, nextSpawnTime;
    int buffer;  // Extra distance outside the screen

    void spawnEnemy(std::vector<Enemy>& enemies);
};

#endif