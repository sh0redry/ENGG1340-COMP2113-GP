#pragma once
#include <vector>

struct Zombie {
    int x;
    int y;
    int health;
    
    Zombie(int x, int y, int health);
    char getDisplayChar() const;
};

class ZombieManager {
private:
    std::vector<Zombie> zombies;
    int enemyInitHP;
    int enemySpeed;
    int enemyMoveCounter;
    int enemySpawnCounter;
    int enemySpawnInterval;
    int spawnProbability;
    
public:
    ZombieManager(int difficulty, int gameLevel);
    
    void update();
    void spawnZombie();
    void moveZombies();
    int processCollision(int x, int y, int damage);
    int getEscapedZombies();
    
    const std::vector<Zombie>& getZombies() const;
    int getInitHP() const;
}; 