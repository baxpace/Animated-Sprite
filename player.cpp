#include "player.h"
// #include "collision.h"

int Player::getX() const { return posX; }
int Player::getY() const { return posY; }

void Player::setX(int x) { posX = x; }
void Player::setY(int y) { posY = y; }


Player::Player(int x, int y)
    : posX(x), posY(y), velX(0), velY(0) {}

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

SDL_Rect Player::getCollisionBox() const {
    return SDL_Rect{ posX, posY, 64, 128 };  // Adjust width/height if player texture change
}

void Player::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Player::move(int windowWidth, int windowHeight) {
    posX += velX;
    posY += velY;

    // Clamp position to window boundaries
    if (posX < 0) posX = 0;
    if (posY < 0) posY = 0;
    if (posX > windowWidth - 50) posX = windowWidth - 64; // Adjust for player if texture size changes
    if (posY > windowHeight - 50) posY = windowHeight - 128;
}

void Player::render(PTexture& texture, SDL_Rect* currentClip) const {
    texture.render(posX, posY, currentClip);
}