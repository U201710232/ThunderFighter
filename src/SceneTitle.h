#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>

class SceneTitle : public Scene{
public:
    SceneTitle();
    ~SceneTitle();
    void handleEvent(SDL_Event* event) override;
    void init() override;
    void clean() override;
    void update(float deltaTime) override;
    void render() override;
private:
    Mix_Music* bgm;
    float timer = 0;

};

#endif