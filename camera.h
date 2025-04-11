#pragma once
#include <SDL2/SDL.h>

class Camera {
public:
    Camera(int screenWidth, int screenHeight);
    void update(int playerX, int playerY);
    void centerOn(float x, float y);
    SDL_Rect getView() const;

private:
    SDL_Rect view;
};