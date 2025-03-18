#ifndef OBJECT_H
#define OBJECT_H
#include <SDL.h>

struct Player{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0,0};
    int width = 0;
    int height = 0;
    int speed = 200;
    Uint32 coolDown = 500;
    Uint32 lastShootTime = 0;
    int currentHealth = 3;
    SDL_Rect getRect();

};

struct ProjectilePlayer{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0,0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;
    SDL_Rect getRect();
};

struct Enemy{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0,0};
    int width = 0;
    int height = 0;
    int speed = 200;
    Uint32 coolDown = 3000;
    Uint32 lastShootTime = 0;
    int currentHealth = 2;
    SDL_Rect getRect();
};

struct projectileEnemy{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0,0};
    SDL_FPoint direction = {0,0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;
    SDL_Rect getRect();
};

struct Explosion
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0,0};
    int width = 0;
    int height = 0;
    int currentFrame = 0;
    int totalFrame = 0;
    Uint32 startTime = 0;
    Uint32 FPS = 10;
    SDL_Rect getDisRect();
    SDL_Rect getRect();
};


#endif