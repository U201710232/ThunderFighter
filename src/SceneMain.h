#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>
#include <map>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


// 声明一个名为Game的类
class Game;

class SceneMain : public Scene {
public:
    // SceneMain();
    ~SceneMain();
    void handleEvent(SDL_Event* event) override;
    void init() override;
    void clean() override;
    void update(float deltaTime) override;
    void render() override;

private:
    Player player;
    Mix_Music* bgm;
    UiHealth uiHealth;
    TTF_Font* scoreFont;
    int score = 0;
    bool isDead; //玩家是否死亡
    //随机数生成器
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    //创建每个物体的模板
    ProjectilePlayer projectileTemplate;
    Enemy enemyTemplate;
    projectileEnemy projectileEnemyTemplate;
    Explosion explosionTemplate;
    Item itemHealthTemplate;
    //创建容器
    std::list<ProjectilePlayer*> projectilesPlayer;
    std::list<Enemy*> enemies;
    std::list<projectileEnemy*> projectilesEnemy;
    std::list<Explosion*> explosions;
    std::list<Item*> items;
    std::map<std::string, Mix_Chunk*> sounds;

    //渲染相关
    void renderProjectilesPlayer();
    void renderEnemies();
    SDL_FPoint getDirection(Enemy* enemy);
    void renderEnemyProjectiles();
    void renderExplosions();
    void renderItems();
    void renderUI();
    //更新相关
    void updateProjectilesPlayer(float deltaTime);
    void updateEnemyProjectiles(float deltaTime);
    void updateEnemies(float deltaTime);
    void updatePlayer();
    void updateExplosions();
    void updateItems(float deltaTime);
    void keyboardControl(float deltaTime);
    void spawnEnemy();
    //其它
    void shootPlayer();
    void shootEnemy(Enemy* enemy);
    void enemyExplode(Enemy* enemy);
    void dropItem(Enemy* enemy);
    void playerGetItem(Item* item);
};

#endif