#include "camera.h"

Camera::Camera(int screenWidth, int screenHeight) {
    view = { 0, 0, screenWidth, screenHeight };
}

void Camera::update(int playerX, int playerY) {
    view.x = playerX - view.w / 2;
    view.y = playerY - view.h / 2;
}

SDL_Rect Camera::getView() const {
    return view;
}

void Camera::centerOn(float x, float y)
{
    view.x = x - view.w / 2;
    view.y = y - view.h / 2;
}