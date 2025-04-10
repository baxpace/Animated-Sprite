#include "player.h"

// SDL_Renderer* gRenderer = nullptr;
int Player::getX() const { return posX; } // Get player X coordinate
int Player::getY() const { return posY; } // Get player Y coordinate

void Player::setX(int x) { posX = x; } // Set player X coordinate position
void Player::setY(int y) { posY = y; } // Set player Y coordinate position

//initialize player position, health etc.
Player::Player(int x, int y)
    : posX(x), posY(y), velX(0), velY(0), health(100) {}

void Player::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:    velY -= PLAYER_SPEED; break;
            case SDLK_DOWN:  velY += PLAYER_SPEED; break;
            case SDLK_LEFT:  velX -= PLAYER_SPEED; break;
            case SDLK_RIGHT: velX += PLAYER_SPEED; break;
        }
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:    velY += PLAYER_SPEED; break;
            case SDLK_DOWN:  velY -= PLAYER_SPEED; break;
            case SDLK_LEFT:  velX += PLAYER_SPEED; break;
            case SDLK_RIGHT: velX -= PLAYER_SPEED; break;
        }
    }
}

void Player::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Player::render(SDL_Renderer* gRenderer, PTexture& texture, SDL_Rect* currentClip) const {
    SDL_Rect healthBar = getHealthBarRect();

    if (isFlashing) {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // White flash
    } else {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Normal red
    }
    // Render the health bar
    SDL_RenderFillRect(gRenderer, &healthBar);  // Directly passing the reference

    texture.render(posX, posY, currentClip);

    // Reset the drawing color to normal (white)
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
}

// void Player::move(int windowWidth, int windowHeight) {
//     posX += velX;
//     posY += velY;

//     // Clamp position to window boundaries
//     if (posX < 0) posX = 0;
//     if (posY < 0) posY = 0;
//     if (posX > windowWidth - 50) posX = windowWidth - 64; // Adjust for player if texture size changes
//     if (posY > windowHeight - 50) posY = windowHeight - 128;
//     if (isFlashing) {
//         flashTimer -= 0.1f; // Reduce timer (adjust based on game loop)
//         if (flashTimer <= 0.0f) {
//             isFlashing = false; // Stop flashing when timer runs out
//         }
//     }
// }

SDL_Rect Player::getCollisionBox() const {
    return SDL_Rect{ posX, posY, 64, 128 };  // Adjust width/height if player texture changes
}

SDL_Rect Player::getHealthBarRect() const {
    int barWidth = 64; // Full width when at full health
    float healthRatio = static_cast<float>(health) / maxHealth;
    return SDL_Rect{
        posX,
        posY,
        static_cast<int>(barWidth * healthRatio),
        10
    };
}

SDL_Rect Player::getHealthBarBorderRect() const {
    return SDL_Rect{
        posX - 1,
        posY - 1,
        66, // barWidth + 2px for left/right border
        12  // bar height + 2px for top/bottom border
    };
}

void Player::takeDamage(int dmg) {
    // Reduce health
    health -= dmg;

    // Clamp to 0
    if (health < 0) {
        health = 0;
    }

    // Trigger the flashing effect
    isFlashing = true;
    flashTimer = SDL_GetTicks();
    lastDamageTime = flashTimer;  // Track the last time the player took damage
}

void Player::reduceHealth(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    lastDamageTime = SDL_GetTicks(); // Log damage time
}

void Player::updateFlash() {
    // Check if the player should stop flashing
    if (isFlashing && SDL_GetTicks() - lastDamageTime >= 200) { // Flash for 200ms
        isFlashing = false; // Reset the flashing state
    }
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

void Player::setMaxHealth(int mh) {
    maxHealth = mh;
}