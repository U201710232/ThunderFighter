#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <random>
SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updateProjectilesPlayer(deltaTime);
    //随机生成敌机
    spawnEnemy();
    updateEnemies(deltaTime);
    updateEnemyProjectiles(deltaTime);
    updatePlayer();
    updateExplosions();
    updateItems(deltaTime);
}

void SceneMain::render()
{
    //渲染玩家子弹
    renderProjectilesPlayer();
    //渲染玩家
    if (!isDead) {
        SDL_Rect playerRect = { static_cast<int>(player.position.x), 
                                static_cast<int>(player.position.y), 
                                player.width, 
                                player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }

    //渲染敌人
    renderEnemies();
    //渲染敌人子弹
    renderEnemyProjectiles();
    //渲染道具
    renderItems();
    //渲染爆炸特效
    renderExplosions();
    
    
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::init()
{
    //载入背景音乐
    bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);
    

    isDead = false;
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture,  NULL, NULL,  &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth()/2 - player.width/2;
    player.position.y = game.getWindowHeight() - player.height;

    //初始化模板
    projectileTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-3.png");
    SDL_QueryTexture(projectileTemplate.texture, NULL, NULL, &projectileTemplate.width, &projectileTemplate.height);
    
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-1.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 2;
    enemyTemplate.height /= 2;

    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 2;
    projectileEnemyTemplate.height /= 2;

    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width/explosionTemplate.height;
    explosionTemplate.width = explosionTemplate.height;

    itemHealthTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    SDL_QueryTexture(itemHealthTemplate.texture, NULL, NULL, &itemHealthTemplate.width, &itemHealthTemplate.height);
    itemHealthTemplate.width /= 2;
    itemHealthTemplate.height /= 2;


}

void SceneMain::clean()
{
    //清理容器
    for (auto &projectile : projectilesPlayer)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    projectilesPlayer.clear();
    for (auto &enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();

    for (auto &projectile : projectilesEnemy)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    projectilesEnemy.clear();
    for (auto &explosion : explosions)
    {
        if (explosion != nullptr)
        {
            delete explosion;
        }
    }
    explosions.clear();
    for(auto &item : items)
    {
        if (item != nullptr)
        {
            delete item;
        }
    }
    items.clear();
    //清理玩家
    if (player.texture != NULL)
    {
        SDL_DestroyTexture(player.texture);
    }
    //清理模板
    if (projectileTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileTemplate.texture);
    }
    if (enemyTemplate.texture != nullptr) 
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
    if (itemHealthTemplate.texture != nullptr) {
        SDL_DestroyTexture(itemHealthTemplate.texture);
    }
    //清理音乐资源
    if(bgm!=nullptr){
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    if (isDead){
        return;
    }
    auto keyboardstate = SDL_GetKeyboardState(NULL);
    if (keyboardstate[SDL_SCANCODE_W])
    {
        player.position.y -= deltaTime*player.speed;
    }
    if (keyboardstate[SDL_SCANCODE_S])
    {
        player.position.y += deltaTime*player.speed;
    }
    if (keyboardstate[SDL_SCANCODE_A])
    {
        player.position.x -= deltaTime*player.speed;
    }
    if (keyboardstate[SDL_SCANCODE_D])
    {
        player.position.x += deltaTime*player.speed;
    }
    //限制飞机的移动范围
    if (player.position.x < 0)
    {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0)
    {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }
    
    //控制子弹发射
    if (keyboardstate[SDL_SCANCODE_J])
    {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown)
        {
            player.lastShootTime = currentTime;
            shootPlayer();
            
        }

    }
}

void SceneMain::shootPlayer()
{
    auto projectile = new ProjectilePlayer(projectileTemplate);
    projectile->position.x = player.position.x + player.width/2 - projectile->width/2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile);
}

void SceneMain::updateProjectilesPlayer(float deltaTime)
{
    int margin = 32;
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it;
        projectile->position.y -= deltaTime*projectile->speed;
        if (projectile->position.y < -margin){
            delete projectile;
            it = projectilesPlayer.erase(it);
        }
        else{
            bool hit = false;
            for (auto enemy : enemies){
                SDL_Rect enemyRect = enemy->getRect();
                SDL_Rect projectileRect = projectile->getRect();
                if (SDL_HasIntersection(&enemyRect, &projectileRect)){
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    hit = true;
                    break;
                }
            }
            if (!hit){
                ++it;
            }
        }
    }
}

void SceneMain::renderProjectilesPlayer()
{
    for (auto projectile : projectilesPlayer)
    {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}

void SceneMain::spawnEnemy()
{
    if (dis(gen) > 1/60.0f){
        return;
    }
    Enemy* enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen)*(game.getWindowWidth()-enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto enemy = *it;
        enemy->position.y += deltaTime*enemy->speed;
        if (enemy->position.y > game.getWindowHeight() + enemy->height)
        {
            delete enemy;
            it = enemies.erase(it);
        }
        else{          
            if ((currentTime - enemy->lastShootTime > enemy->coolDown) && (isDead == false))
            {
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            if (enemy->currentHealth <= 0){
                enemyExplode(enemy);
                it = enemies.erase(it);
            }
            else{
                ++it;
            }
            
            
        }
    }
}

void SceneMain::renderEnemies()
{
    for (auto enemy : enemies)
    {
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy* enemy)
{
    auto protile = new projectileEnemy(projectileEnemyTemplate);
    protile->position.x = enemy->position.x + enemy->width/2 - protile->width/2;
    protile->position.y = enemy->position.y + enemy->height/2 - protile->height/2;
    protile->direction = getDirection(enemy);
    projectilesEnemy.push_back(protile);
}

SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    auto x = player.position.x + player.width/2 - enemy->position.x - enemy->width/2;
    auto y = player.position.y + player.height/2 - enemy->position.y - enemy->height/2;
    auto length = sqrt(x*x + y*y);
    x = x/length;
    y = y/length;
    return SDL_FPoint({x,y});
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    auto margin = 32;
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        auto projectile = *it;
        projectile->position.x += deltaTime*projectile->speed*projectile->direction.x;
        projectile->position.y += deltaTime*projectile->speed*projectile->direction.y;
        if (projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.y < -projectile->height - margin ||
            projectile->position.x > game.getWindowWidth() + margin ||
            projectile->position.x < -projectile->width - margin)
        {   
            delete projectile;
            it = projectilesEnemy.erase(it);
        }
        else{
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height
            };
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            if (SDL_HasIntersection(&projectileRect, &playerRect) && isDead == false)
            {
                player.currentHealth -= projectile->damage;
                delete projectile;
                it = projectilesEnemy.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for (auto projectile : projectilesEnemy)
    {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        // SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI -90;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
        
    }
}

void SceneMain::enemyExplode(Enemy *enemy)
{
    auto currentTime = SDL_GetTicks();
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width/2 - explosion->width/2;
    explosion->position.y = enemy->position.y + enemy->height/2 - explosion->height/2;
    explosion->startTime = currentTime;
    explosions.push_back(explosion);
    if (dis(gen)<0.8){
        dropItem(enemy);
    }
    delete enemy;
}

void SceneMain::updatePlayer()
{
    if ( isDead ){
        return;
    }
    if (player.currentHealth <= 0){
        //game over
        isDead = true;
            auto currentTime = SDL_GetTicks();
            auto explosion = new Explosion(explosionTemplate);
            explosion->position.x = player.position.x + player.width/2 - explosion->width/2;
            explosion->position.y = player.position.y + player.height/2 - explosion->height/2;
            explosion->startTime = currentTime;
            explosions.push_back(explosion);
        return;
    }
    for(auto enemy : enemies)
    {
        SDL_Rect playerRect = player.getRect();
        SDL_Rect enemyRect = enemy->getRect();

        if (SDL_HasIntersection(&playerRect, &enemyRect))
        {
            player.currentHealth -= 1;
            enemy->currentHealth = 0;
        }
    }
}

void SceneMain::updateExplosions()
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        auto explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime)*explosion->FPS / 1000;
        if (explosion->currentFrame >= explosion->totalFrame){
            delete explosion;
            it = explosions.erase(it);
        }
        else{
            ++it;
        }

    }

}

void SceneMain::renderExplosions()
{
    for(auto explosion : explosions)
    {
        SDL_Rect explosionDisRect = explosion->getDisRect();
        SDL_Rect explosionRect = explosion->getRect();

        SDL_RenderCopy(game.getRenderer(), explosion->texture, &explosionDisRect, &explosionRect);
    }
}

void SceneMain::dropItem(Enemy *enemy)
{
    auto item = new Item(itemHealthTemplate);
    item->position.x = enemy->position.x + enemy->width/2 - item->width/2;
    item->position.y = enemy->position.y + enemy->height/2 - item->height/2;
    auto angle = dis(gen)*360;
    item->direction.x = cos(angle);
    item->direction.y = sin(angle);
    items.push_back(item);
}

void SceneMain::updateItems(float deltaTime)
{
    for(auto it = items.begin(); it != items.end();)
    {
        auto item = *it;
        //如果超出屏幕范围就删除
        if (item->position.x < -item->width || 
            item->position.x > game.getWindowWidth() || 
            item->position.y < -item->height || 
            item->position.y > game.getWindowHeight())
        {
            delete item;
            it = items.erase(it);
        }
        else
        {   
            //更新位置
            item->position.x += item->direction.x * item->speed * deltaTime;
            item->position.y += item->direction.y * item->speed * deltaTime;
            //判断反弹
            if (item->bounceTimes > 0){
                if (item->position.x < 0 || item->position.x > game.getWindowWidth() - item->width)
                {
                    item->direction.x = -item->direction.x;
                    item->bounceTimes--;
                }
                if (item->position.y < 0 || item->position.y > game.getWindowHeight() - item->height)
                {
                    item->direction.y = -item->direction.y;
                    item->bounceTimes--;
                }
            }
            
            
            auto playerRect = player.getRect();
            auto itemRect = item->getRect();
            if (SDL_HasIntersection(&playerRect, &itemRect))
            {
                playerGetItem(item);
                delete item;
                it = items.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    
}

void SceneMain::renderItems()
{
    for(auto &item : items)
    {
        SDL_Rect itemRect = item->getRect();
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
    }
}

void SceneMain::playerGetItem(Item *item)
{
    if(item->type == ItemType::Health)
    {
        player.currentHealth += 1;
        if (player.currentHealth > player.maxHealth)
        {
            player.currentHealth = player.maxHealth;
        }
    }
}
