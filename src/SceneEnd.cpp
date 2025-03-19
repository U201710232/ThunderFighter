#include "SceneEnd.h"
#include "string"
#include "Game.h"
#include "SceneMain.h"

void SceneEnd::init()
{
    endBgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (!endBgm){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "SDL", "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(endBgm, -1);

    isTyping = true;
    if(!SDL_IsTextInputActive()){
        SDL_StartTextInput();
    }
    if(!SDL_IsTextInputActive()){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "SDL", "Failed to start text input: %s", SDL_GetError());
    }
    
}

void SceneEnd::update(float deltaTime)
{
    blinkTimer += deltaTime;
    if (blinkTimer > 1.0f){
        blinkTimer -= 1.0f;
    }
}

void SceneEnd::render()
{
    if (isTyping){
        renderPhase1();
    }
    else{
        renderPhase2();
    }
}

void SceneEnd::clean()
{
    if(endBgm != nullptr){
        Mix_HaltMusic();
        Mix_FreeMusic(endBgm);
    }
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    
    if (isTyping){
        if(event->type == SDL_TEXTINPUT){
            name+= event->text.text;
        }
        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.scancode == SDL_SCANCODE_RETURN){
                isTyping = false;
                SDL_StopTextInput();
                if (name == ""){
                    name = u8"无名氏";
                }
                game.insertLeaderBoard(game.getFinalScore(), name);
            }
            if(event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
                if (name != ""){
                    removeLastUTF8Char(name);
                }
            }
        }

    }
    else{
        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.scancode == SDL_SCANCODE_J){
                auto sceneMain = new SceneMain();
                game.changeScene(sceneMain);
            }
        }
        //TODO
    }
}

void SceneEnd::renderPhase1()
{
    int score = game.getFinalScore();
    std::string scoreText = u8"你的得分是:" + std::to_string(score);
    std::string gameOver = u8"Game Over";
    std::string instructionText = u8"请输入你的名字，按回车确认:";
    game.renderTextCentered(scoreText, 0.2, false);
    game.renderTextCentered(gameOver, 0.4, false);
    game.renderTextCentered(instructionText, 0.6, false);
    if (name != ""){
        SDL_Point p = game.renderTextCentered(name, 0.8, false);
        if (blinkTimer < 0.5f){
            game.renderTextPoint(u8"_", p.x, p.y,  false);
        }
    }
    else{
        if (blinkTimer < 0.5f){
            game.renderTextCentered(u8"_", 0.8, false);;
        }
        
    }
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered(u8"得分榜" , 0.1, true);
    float posY = 0.2 * game.getWindowHeight();
    int rank = 1;
    int x = 100;
    int y = static_cast<int>(posY);
    for (auto item: game.getLeaderBoard()){
        std::string name = std::to_string(rank)+ ". " + item.second;
        std::string score = std::to_string(item.first);

        game.renderTextPoint(name, x, y, false);
        game.renderTextPoint(score, x, y, false, false);
        y += 50;
        rank++;
    }
    if(blinkTimer < 0.5f){
        game.renderTextCentered(u8"按J键重新开始游戏", 0.85, false);
    }
}

void SceneEnd::removeLastUTF8Char(std::string &str)
{
    if (str.empty()) return;
    while ((str.back() & 0b11000000) == 0b10000000) {//中文字符的后续字节
        str.pop_back();
    }
    str.pop_back();
}
