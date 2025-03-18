#include "Game.h"
#include "SceneMain.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

void Game::backgroundUpdate(float deltaTime)
{
    nearStars.offset += nearStars.speed * deltaTime;
    if(nearStars.offset>0){
        nearStars.offset -= nearStars.height;
    }
    farStars.offset += farStars.speed * deltaTime;
    if(farStars.offset>0){
        farStars.offset -= farStars.height;
    }
}

void Game::renderBackground()
{
    int posY = static_cast<int>(nearStars.offset);
    for (posY; posY < windowHeight; posY += nearStars.height)
    {
        for (int posX=0; posX < windowWidth; posX += nearStars.width)
        {
            SDL_Rect dstRect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, NULL, &dstRect);
        }
    }
    posY = static_cast<int>(farStars.offset);
    for (posY; posY < windowHeight; posY += farStars.height)
    {
        for (int posX=0; posX < windowWidth; posX += farStars.width)
        {
            SDL_Rect dstRect = {posX, posY, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, NULL, &dstRect);
        }
    }
}

Game::Game()
{
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while(isRunning)
    {   auto frameStart = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);
        update(deltaTime);
        render();
        auto frameEnd = SDL_GetTicks();
        auto diff = frameEnd - frameStart;
        if (diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        }
        else
        {
            deltaTime = diff / 1000.0f;
        }
    }
}

void Game::init()
{
    frameTime = 1000 / FPS;
    //SDL初始化
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    //创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    //初始化SDL_Image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Image could not initialize! SDL_Error: %s\n", IMG_GetError());
        isRunning = false;
    }
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Mixer could not initialize! SDL_Error: %s\n", Mix_GetError());
        isRunning = false;
    }
    //打开音频设备
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Mixer could not open audio device! SDL_Error: %s\n", Mix_GetError());
        isRunning = false;  
    }
    //设置音效channel数量
    Mix_AllocateChannels(32);

    //设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME/4);
    Mix_Volume(-1, MIX_MAX_VOLUME/8);


    //初始化背景卷轴
    nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
    nearStars.height /= 2;
    nearStars.width /=2;
    farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.height /= 2;
    farStars.width /=2;
    farStars.speed = 15;

    currentScene = new SceneMain();
    currentScene->init();
    
    
    
    
    
    
    

}

void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    if(nearStars.texture != nullptr){
        SDL_DestroyTexture(nearStars.texture);
    }
    if(farStars.texture != nullptr){
        SDL_DestroyTexture(farStars.texture);
    }
    //清理SDL_Image
    IMG_Quit();
    //清理音乐
    Mix_CloseAudio();
    Mix_Quit();
    //清理SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void Game::changeScene(Scene* scene)
{
    if(currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
        currentScene = scene;
        currentScene->init();
    }
}

void Game::handleEvent(SDL_Event * event)
{
    while(SDL_PollEvent(event))
        {
            if(event->type == SDL_QUIT)
            {
                isRunning = false;
            }
            currentScene->handleEvent(event);
        }
}

void Game::update(float deltaTime)
{
    backgroundUpdate(deltaTime);
    currentScene->update(deltaTime);
}

void Game::render()
{
    //清空
    SDL_RenderClear(renderer);
    //渲染星空背景
    renderBackground();

    currentScene->render();
    //显示更新
    SDL_RenderPresent(renderer);
}
