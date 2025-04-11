#include "player.h"
#include "sprite_data.h"

// SDL_Renderer* gRenderer = nullptr;
int Player::getX() const { return posX; } // Get player X coordinate
int Player::getY() const { return posY; } // Get player Y coordinate

void Player::setX(int x) { posX = x; } // Set player X coordinate position
void Player::setY(int y) { posY = y; } // Set player Y coordinate position

//initialize player position, health etc.
Player::Player(int x, int y)
    : posX(x), posY(y), velX(0), velY(0), health(100) {}

void Player::handleInput(const Uint8* keyStates) {
    velX = 0;
    velY = 0;

    if (keyStates[SDL_SCANCODE_UP]) {
        velY = -PLAYER_SPEED;
        facingDirection = UP;
    } else if (keyStates[SDL_SCANCODE_DOWN]) {
        velY = PLAYER_SPEED;
        facingDirection = DOWN;
    }

    if (keyStates[SDL_SCANCODE_LEFT]) {
        velX = -PLAYER_SPEED;
        facingDirection = LEFT;
    } else if (keyStates[SDL_SCANCODE_RIGHT]) {
        velX = PLAYER_SPEED;
        facingDirection = RIGHT;
    }
}

SDL_Rect* Player::getCurrentAnimationClip(int frame) {
    int index = frame / 4;

    // Wrap around with WALKING_ANIMATION_FRAMES
    index %= WALKING_ANIMATION_FRAMES;

    if (velY < 0) {
        return &gSpriteClipsUp[index];
    } else if (velY > 0) {
        return &gSpriteClipsDown[index];
    } else if (velX < 0) {
        return &gSpriteClipsLeft[index];
    } else if (velX > 0) {
        return &gSpriteClipsRight[index];
    }

    // Default: return standing frame
    return &gSpriteClipsDown[1];
}

void Player::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

// void Player::render(SDL_Renderer* gRenderer, PTexture& texture, SDL_Rect* currentClip) const {
//     SDL_Rect healthBar = getHealthBarRect();

//     if (isFlashing) {
//         SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // White flash
//     } else {
//         SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Normal red
//     }
//     // Render the health bar
//     SDL_RenderFillRect(gRenderer, &healthBar);  // Directly passing the reference

//     texture.render(posX, posY, currentClip);

//     // Reset the drawing color to normal (white)
//     SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
// }

// void Player::render(SDL_Renderer* gRenderer, PTexture& texture, SDL_Rect* currentClip, const SDL_Rect& cameraView) const {
//     // Apply camera offset to player position
//     int renderX = static_cast<int>(posX) - cameraView.x;
//     int renderY = static_cast<int>(posY) - cameraView.y;

//     // Adjust health bar position relative to camera
//     SDL_Rect healthBar = getHealthBarRect();
//     healthBar.x -= cameraView.x;
//     healthBar.y -= cameraView.y;

//     // Flashing effect
//     if (isFlashing) {
//         SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // White flash
//     } else {
//         SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Normal red
//     }

//     SDL_RenderFillRect(gRenderer, &healthBar);  // Render health bar

//     // Render the player sprite with offset
//     texture.render(renderX, renderY, currentClip);

//     // Reset drawing color to default
//     SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
// }

void Player::render(SDL_Renderer* gRenderer, PTexture& texture, SDL_Rect* currentClip, const SDL_Rect& cameraView) const {
    // Apply camera offset to player position
    int renderX = static_cast<int>(posX) - cameraView.x;
    int renderY = static_cast<int>(posY) - cameraView.y;

    // Render player sprite with camera offset
    texture.render(renderX, renderY, currentClip);
    
    // Adjust health bar and border position relative to camera
    SDL_Rect healthBar = getHealthBarRect();
    SDL_Rect healthBorder = getHealthBarBorderRect();

    healthBar.x -= cameraView.x;
    healthBar.y -= cameraView.y;
    healthBorder.x -= cameraView.x;
    healthBorder.y -= cameraView.y;

    // Render health bar border first (black box behind red bar)
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Black border
    SDL_RenderFillRect(gRenderer, &healthBorder);

    // Flashing effect for red bar
    if (isFlashing) {
        SDL_SetRenderDrawColor(gRenderer, 255, 100, 100, 255); // Light red flash
    } else {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Normal red
    }

    SDL_RenderFillRect(gRenderer, &healthBar); // Render health bar

    // Reset drawing color
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
}

void Player::move() {
    posX += velX;
    posY += velY;
}

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