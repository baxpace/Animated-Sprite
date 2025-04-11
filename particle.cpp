#include "particle.h"
#include <cstdlib>
#include <vector>

std::vector<Particle> particles;							// a vector for spawned particles
std::vector<Particle> particlePool;
const int MAX_PARTICLES = 1000;

Particle::Particle(int x, int y, SDL_Color color)
    : posX(x), posY(y), color(color), age(0.0f)
{
    velX = static_cast<float>((rand() % 50) - 25); // -75 to +75
    velY = static_cast<float>((rand() % 50) - 25);
    lifetime = 2.5f + static_cast<float>(rand() % 100) / 50.0f; // ~3.5–4.5 seconds
    size = 1.5f + static_cast<float>(rand() % 5); // 4–7 pixels
    angle = static_cast<float>(rand() % 360);
    rotationSpeed = static_cast<float>((rand() % 200) - 100); // -100° to +100° per sec
    alpha = 255;
}

void Particle::update(float deltaTime) {
    // Update position based on velocity
    posX += velX * deltaTime;
    posY += velY * deltaTime;

    // Update age and check lifetime
    age += deltaTime;
    if (age >= lifetime) {
        alpha = 0; // Mark particle as dead by setting alpha to 0
    }

    // Optional: Add other updates like rotation speed, size change, etc.
    angle += rotationSpeed * deltaTime; // Update rotation
}

bool Particle::isAlive() const {
    return age < lifetime;
}

void Particle::render(SDL_Renderer* renderer, const SDL_Rect& cameraView) const {
    // Offset the position by the camera view to stay in sync with world movement
    SDL_Rect rect = {
        static_cast<int>(posX - cameraView.x - size / 2), // Center and offset by camera
        static_cast<int>(posY - cameraView.y - size / 2),
        static_cast<int>(size),
        static_cast<int>(size)
    };

    // Create a solid-colored texture once and reuse it
    static SDL_Texture* particleTex = nullptr;

    if (!particleTex) {
        particleTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
        SDL_SetTextureBlendMode(particleTex, SDL_BLENDMODE_BLEND);
    }

    // Set the texture to the particle's color and alpha
    SDL_SetRenderTarget(renderer, particleTex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    // Render the particle with rotation
    SDL_RenderCopyEx(renderer, particleTex, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
}

void spawnParticle(int x, int y, SDL_Color color) {
    for (auto& p : particlePool) {
        if (!p.isAlive()) {
            p.reset(x, y, color);
            return;
        }
    }

    if (particlePool.size() < MAX_PARTICLES) {
        particlePool.emplace_back(x, y, color);
    }
}

void updateAndRenderParticles(SDL_Renderer* renderer, float deltaTime, const SDL_Rect& cameraView) {
    for (auto& p : particlePool) {
        if (p.isAlive()) {
            p.update(deltaTime);
            p.render(renderer, cameraView); // ✅ Now matches the new function signature
        }
    }
}

void Particle::reset(int x, int y, SDL_Color color) {
    posX = x;
    posY = y;
    this->color = color;
    velX = (rand() % 100 - 50) / 10.0f; // Random X velocity
    velY = (rand() % 100 - 50) / 10.0f; // Random Y velocity
    lifetime = rand() % 5 + 2;          // Random lifetime between 2 and 7 seconds
    age = 0;
    size = rand() % 5 + 2;              // Random size for particle
    alpha = 255;                         // Full opacity
    active = true;                       // Mark particle as active
}

// Initialize particle pool
void initParticlePool() {
    particlePool.resize(MAX_PARTICLES);
    for (auto& particle : particlePool) {
        particle.reset(0, 0, {255, 255, 255, 255}); // Reset all particles to inactive state
    }
}

// Get an inactive particle from the pool
Particle* getInactiveParticle(int x, int y, SDL_Color color) {
    for (auto& particle : particlePool) {
        if (!particle.isAlive()) {
            particle.reset(x, y, color);
            return &particle;
        }
    }
    return nullptr; // No inactive particle available
}