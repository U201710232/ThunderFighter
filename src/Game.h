#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Game
{
public:
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    };

    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    void handleEvent(SDL_Event *event);
    void update(float deltaTime);
    void render();
    //渲染工具函数
    void renderTextCentered(std::string text, float posY, bool istitle);
    //getter
    SDL_Window* getWindow() { return window; }  
    SDL_Renderer* getRenderer() { return renderer; }  
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }


private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    bool isRunning = true;
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    TTF_Font* titleFont = nullptr;
    TTF_Font* textFont = nullptr;
    int FPS = 60;
    Uint32 frameTime;
    float deltaTime;
    Background nearStars;
    Background farStars;
    void backgroundUpdate(float deltaTime);
    void renderBackground();
};

#endif