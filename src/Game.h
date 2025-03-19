#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <map>

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
    SDL_Point renderTextCentered(std::string text, float posY, bool istitle);
    SDL_Point renderTextPoint(std::string text, int x, int y, bool istitle, bool isLeft = true);
    //setter
    void setFinalScore(int score) { finalScore = score; }
    //getter
    int getFinalScore() { return finalScore; }

    
    SDL_Window* getWindow() { return window; }  
    SDL_Renderer* getRenderer() { return renderer; }  
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }
    std::multimap<int, std::string, std::greater<int>>& getLeaderBoard() { return leaderBoard; }
    void insertLeaderBoard(int score, std::string name);  

private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    bool isRunning = true;
    bool fullScreen = false;
    int finalScore = 0;
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
    std::multimap<int, std::string, std::greater<int>> leaderBoard;
    Background nearStars;
    Background farStars;
    void backgroundUpdate(float deltaTime);
    void renderBackground();
    void saveData();
    void loadData();

};

#endif