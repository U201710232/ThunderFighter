#include "SceneTitle.h"
#include "Game.h"
#include "SceneMain.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::handleEvent(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.scancode == SDL_SCANCODE_J){
            auto sceneMain = new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}

void SceneTitle::init()
{
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::clean()
{
    if (bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneTitle::update(float deltaTime)
{
    timer += deltaTime;
    if (timer > 1.0f){
        timer -= 1.0f;
    }
}

void SceneTitle::render()
{
    //渲染标题文字
    std::string  titleText = u8"太空战机";
    game.renderTextCentered(titleText,0.37, true);
    //渲染普通文字
    if (timer < 0.5f){
        titleText = u8"按J开始游戏";
        game.renderTextCentered(titleText,0.73, false);
    }
    
}
