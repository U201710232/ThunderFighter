#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>

class Game;

class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event* event) override;
    void init() override;
    void clean() override;
    void keyboardControl(float deltaTime);
    void shootPlayer();
    void updateProjectilesPlayer(float deltaTime);
    void renderProjectilesPlayer();
    void spawnEnemy();
    void updateEnemies(float deltaTime);
    void renderEnemies();
    void shootEnemy(Enemy* enemy);
    SDL_FPoint getDirection(Enemy* enemy);
    void updateEnemyProjectiles(float deltaTime);
    void renderEnemyProjectiles();
    void enemyExplode(Enemy* enemy);
    void updatePlayer();
    void updateExplosions();
    void renderExplosions();
    void dropItem(Enemy* enemy);
    void updateItems(float deltaTime);
    void renderItems();
    void playerGetItem(Item* item);
    void renderHealthUI();
private:
    Player player;
    Game &game;
    Mix_Music* bgm;
    UiHealth uiHealth;
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
};

#endif