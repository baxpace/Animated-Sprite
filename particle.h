#pragma once
#include <SDL2/SDL.h>

class Particle {
    public:
        Particle(int x, int y, SDL_Color color);
        void update(float deltaTime);
        void render(SDL_Renderer* renderer) const;
        bool isAlive() const;

    private:
        float posX, posY;
        float velX, velY;
        SDL_Color color;
        float lifetime;  
        float age;
        float size;           
        float angle;         // Rotation angle
        float rotationSpeed; // Rotation per second
        Uint8 alpha;         // Current transparency  
};