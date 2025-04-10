#include "particle.h"
#include <cstdlib>

Particle::Particle(int x, int y, SDL_Color color)
    : posX(x), posY(y), color(color), age(0.0f)
{
    velX = static_cast<float>((rand() % 50) - 25); // -75 to +75
    velY = static_cast<float>((rand() % 50) - 25);
    lifetime = 3.5f + static_cast<float>(rand() % 100) / 100.0f; // ~3.5–4.5 seconds
    size = 1.25f + static_cast<float>(rand() % 3); // 4–7 pixels
}

void Particle::update(float deltaTime) {
    age += deltaTime;

    // Move based on velocity
    posX += velX * deltaTime;
    posY += velY * deltaTime;
}

bool Particle::isAlive() const {
    return age < lifetime;
}

void Particle::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = {
        static_cast<int>(posX - size / 2),
        static_cast<int>(posY - size / 2),
        static_cast<int>(size),
        static_cast<int>(size)
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}