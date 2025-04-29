#pragma once
#include "Weapon.h"
#include "Zombie.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../UI/Animation.h"
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
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
    
    Player& player;
    
    void processInput();
    void update();
    void draw() const;
    bool isTimeUp() const;
    int getRemainingTime() const;
    
public:
    Combat(Player& player, const WeekCycle& weekCycle);
    bool run();
}; 