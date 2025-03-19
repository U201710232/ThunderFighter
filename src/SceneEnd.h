#ifndef SceneEnd_H
#define SceneEnd_H

#include "Scene.h"
class SceneEnd : public Scene{
public:
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean() override;
    virtual void handleEvent(SDL_Event* event);

private:
    float blinkTimer = 0;
    bool isTyping = true;
    std::string name;
    void renderPhase1();
    void renderPhase2();
    void removeLastUTF8Char(std::string &str);
};

#endif