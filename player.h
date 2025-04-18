#pragma once

#include <SDL2/SDL.h>
#include "textures.h"

class Player {
public:
    Player(int x, int y);
    void handleEvent(const SDL_Event& e);
    void move();
    void render(SDL_Renderer* gRenderer, PTexture& texture, SDL_Rect* currentClip, const SDL_Rect& cameraView) const;
    void setPosition(int x, int y);
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    SDL_Rect getCollisionBox() const;
    void takeDamage(int dmg);
    void reduceHealth(int amount);
    int getHealth() const;
    bool getIsFlashing() const {return isFlashing;} 
    int getMaxHealth() const;
    void setMaxHealth(int mh);
    void updateFlash();
    void handleInput(const Uint8* keyStates);
    SDL_Rect* getCurrentAnimationClip(int frame);
    SDL_Rect getHealthBarRect() const;
    SDL_Rect getHealthBarBorderRect() const;
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction facingDirection = DOWN;

private:
    int health;
    int posX = 0, posY = 0;
    int velX = 0, velY = 0;
    int maxHealth = 100; 
    bool recentlyHit = false;
    Uint32 lastDamageTime;
    bool isFlashing = false; // Flag for the flashing effect
    Uint32 flashTimer; // Timer for the flashing effect
    const int PLAYER_SPEED = 8;
};