#pragma once
#include "Weapon.h"
#include "Zombie.h"
#include "Terminal.h"
#include "UI.h"
#include <vector>
#include <chrono>

class Combat {
private:
    static const int WIDTH = 25;
    static const int HEIGHT = 25;
    static const char PLAYER_CHAR = 'A';
    static const char BULLET_CHAR = '|';
    
    Weapon weapon;
    ZombieManager zombieManager;
    std::vector<std::pair<int, int>> bullets;
    
    int playerX;
    int playerY;
    int HP;
    int initialHP;
    int gameDuration;
    std::chrono::steady_clock::time_point startTime;
    
    // 发射冷却相关
    static const int FIRE_COOLDOWN_MS = 200; // 发射冷却时间（毫秒）
    std::chrono::steady_clock::time_point lastFireTime;
    bool canFire() const;
    void fireBullets();
    
    void processInput();
    void update();
    void draw() const;
    bool isTimeUp() const;
    int getRemainingTime() const;
    
public:
    Combat(int weaponLevel, int difficulty, int gameLevel, int people);
    bool run();
}; 