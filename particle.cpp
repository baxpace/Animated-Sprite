#include "particle.h"
#include <cstdlib>
#include <vector>

Particle::Particle(int x, int y, SDL_Color color)
    : posX(x), posY(y), color(color), age(0.0f)
{
    velX = static_cast<float>((rand() % 25) - 13); // -75 to +75
    velY = static_cast<float>((rand() % 25) - 13);
    lifetime = 5.5f + static_cast<float>(rand() % 100) / 50.0f; // ~3.5–4.5 seconds
    size = 1.5f + static_cast<float>(rand() % 5); // 4–7 pixels
    angle = static_cast<float>(rand() % 360);
    rotationSpeed = static_cast<float>((rand() % 200) - 100); // -100° to +100° per sec
    alpha = 255;
}

void Particle::update(float deltaTime) {
    age += deltaTime;

    // Move based on velocity
    posX += velX * deltaTime;
    posY += velY * deltaTime;
    angle += rotationSpeed * deltaTime;

    // Fade out
    float lifeRatio = 10.25f - (age / lifetime);
    alpha = static_cast<Uint8>(lifeRatio * 255.0f);
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

    // Create a solid-colored texture once and reuse it if possible
    SDL_Texture* particleTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetTextureBlendMode(particleTex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, particleTex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopyEx(renderer, particleTex, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    SDL_DestroyTexture(particleTex); // Optional: cache and reuse this if perf is a concern
}