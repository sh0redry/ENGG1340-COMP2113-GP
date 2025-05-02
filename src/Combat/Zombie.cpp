/**
 * @file Zombie.cpp
 * @brief Implementation of the zombie enemy system
 * @details Manages zombie behavior, attributes, and combat mechanics
 *          including movement patterns and difficulty scaling.
 */

#include "Zombie.h"
#include "../Utils/Random.h"

/**
 * @brief Constructor for a Zombie
 * @param x Initial X coordinate
 * @param y Initial Y coordinate
 * @param health Initial health points
 */
Zombie::Zombie(int x, int y, int health) : x(x), y(y), health(health) {}

/**
 * @brief Gets the display character for the zombie
 * @return '*' for healthy zombies (health >= 45), '+' for damaged zombies
 */
char Zombie::getDisplayChar() const {
    return health >= 45 ? '*' : '+';
}

/**
 * @brief Constructor for ZombieManager
 * @param difficulty Current game difficulty (1-3)
 * @param gameLevel Current game level (1-5)
 */
ZombieManager::ZombieManager(int difficulty, int gameLevel) 
    : enemySpeed(20), enemyMoveCounter(0), enemySpawnCounter(0), 
      enemySpawnInterval(30), spawnProbability(15) {
    
    // Set initial HP based on difficulty and level
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

/**
 * @brief Updates all zombies' states
 * Handles zombie movement and spawning based on timers
 */
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

/**
 * @brief Spawns a new zombie if conditions are met
 * Randomly spawns zombies based on probability
 */
void ZombieManager::spawnZombie() {
    if (Random::Chance(spawnProbability / 100.0f)) {
        int x = Random::Range(0, 24); // WIDTH - 1
        zombies.emplace_back(x, 0, enemyInitHP);
    }
}

/**
 * @brief Moves all zombies according to their behavior
 * Moves zombies downward (increasing Y coordinate)
 */
void ZombieManager::moveZombies() {
    for (auto& zombie : zombies) {
        zombie.y++;
    }
}

/**
 * @brief Processes collision between a position and zombies
 * @param x X coordinate to check
 * @param y Y coordinate to check
 * @param damage Damage to apply on collision
 * @return Total damage dealt to zombies
 */
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

/**
 * @brief Gets the number of zombies that have escaped
 * Removes escaped zombies and returns their count
 * @return Number of escaped zombies
 */
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

/**
 * @brief Gets the collection of active zombies
 * @return Reference to the vector of zombies
 */
const std::vector<Zombie>& ZombieManager::getZombies() const {
    return zombies;
}

/**
 * @brief Gets the initial health points for new zombies
 * @return Initial health points value
 */
int ZombieManager::getInitHP() const {
    return enemyInitHP;
} 