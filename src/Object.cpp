#include "Object.h"

SDL_Rect Player::getRect()
{
    return SDL_Rect{static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    width, 
                    height};
}

SDL_Rect Enemy::getRect()
{
    return SDL_Rect{static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    width, 
                    height};
}

SDL_Rect ProjectilePlayer::getRect()
{
    return SDL_Rect{static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    width, 
                    height};
}

SDL_Rect projectileEnemy::getRect()
{
    return SDL_Rect{static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    width, 
                    height};
}

SDL_Rect Explosion::getDisRect()
{
    return SDL_Rect{currentFrame* width,
                    0,
                    width, 
                    height};
}

SDL_Rect Explosion::getRect(float scale)
{
    return SDL_Rect{static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    static_cast<int>(width * scale), 
                    static_cast<int>(height * scale)};
}

SDL_Rect Item::getRect()
{
    return SDL_Rect{static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    width, 
                    height};
}


