#pragma once

#include <SDL2/SDL.h>
#include "textures.h"

class Player {
public:
    Player(int x, int y);
    void handleEvent(const SDL_Event& e);
    void move(int windowWidth, int windowHeight);
    void render(PTexture& texture, SDL_Rect* currentClip = nullptr) const;
    void setPosition(int x, int y);
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    SDL_Rect getCollisionBox() const;
    void reduceHealth(int amount);
    Uint32 lastDamageTime = 0; // Tracks when damage occurred
    bool isFlashing() const;
    int getHealth() const;
    SDL_Rect getHealthBarRect() const;
    SDL_Rect getHealthBarBorderRect() const;

private:
    int health;
    int posX = 0, posY = 0;
    int velX = 0, velY = 0;
    const int PLAYER_SPEED = 8;
};