#include "Zombie.h"
#include "../Utils/Random.h"

Zombie::Zombie(int x, int y, int health) : x(x), y(y), health(health) {}

char Zombie::getDisplayChar() const {
    return health >= 45 ? '*' : '+';
}

ZombieManager::ZombieManager(int difficulty, int gameLevel) 
    : enemySpeed(20), enemyMoveCounter(0), enemySpawnCounter(0), 
      enemySpawnInterval(30), spawnProbability(15) {
    
    // 根据难度和等级设置初始血量
    if (difficulty == 1) {
        int enemyHParr[5] = {36, 45, 54, 63, 72};
        enemyInitHP = enemyHParr[gameLevel - 1];
    } else if (difficulty == 2) {
        int enemyHParr[5] = {40, 50, 60, 70, 80};
        enemyInitHP = enemyHParr[gameLevel - 1];
    } else if (difficulty == 3) {
        int enemyHParr[5] = {44, 55, 66, 77, 88};
        enemyInitHP = enemyHParr[gameLevel - 1];
    }
}

void ZombieManager::update() {
    enemyMoveCounter++;
    if (enemyMoveCounter >= enemySpeed) {
        moveZombies();
        enemyMoveCounter = 0;
    }
    
    enemySpawnCounter++;
    if (enemySpawnCounter >= enemySpawnInterval) {
        spawnZombie();
        enemySpawnCounter = 0;
    }
}

void ZombieManager::spawnZombie() {
    if (Random::Chance(spawnProbability / 100.0f)) {
        int x = Random::Range(0, 24); // WIDTH - 1
        zombies.emplace_back(x, 0, enemyInitHP);
    }
}

void ZombieManager::moveZombies() {
    for (auto& zombie : zombies) {
        zombie.y++;
    }
}

int ZombieManager::processCollision(int x, int y, int damage) {
    int totalDamage = 0;
    for (auto it = zombies.begin(); it != zombies.end();) {
        if (it->x == x && it->y == y) {
            it->health -= damage;
            if (it->health <= 0) {
                totalDamage += damage;
                it = zombies.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    return totalDamage;
}

int ZombieManager::getEscapedZombies() {
    int escaped = 0;
    for (auto it = zombies.begin(); it != zombies.end();) {
        if (it->y >= 25) { // HEIGHT
            escaped++;
            it = zombies.erase(it);
        } else {
            ++it;
        }
    }
    return escaped;
}

const std::vector<Zombie>& ZombieManager::getZombies() const {
    return zombies;
}

int ZombieManager::getInitHP() const {
    return enemyInitHP;
} 