#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Particle {
    public:
        Particle(int x = 0, int y = 0, SDL_Color color = {255, 255, 255, 255}); // Default constructor for reuse        
        void update(float deltaTime);
        void render(SDL_Renderer* renderer) const;
        bool isAlive() const;
        void reset(int x, int y, SDL_Color color); // Reset particle state for reuse
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
        bool active;         // Track if the particle is active
};

extern std::vector<Particle> particles;	
extern std::vector<Particle> particlePool;
extern const int MAX_PARTICLES;
void spawnParticle(int x, int y, SDL_Color color);
void updateAndRenderParticles(SDL_Renderer* renderer, float deltaTime);
