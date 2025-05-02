/**
 * @file Zombie.h
 * @brief Zombie system for enemy management
 * 
 * This file defines the Zombie struct and ZombieManager class, which handle
 * zombie entities and their behavior in the game. It manages zombie spawning,
 * movement, health, and interactions with the player.
 * 
 * The Zombie system features:
 * - Individual zombie tracking
 * - Spawn system with probability control
 * - Movement mechanics
 * - Health management
 * - Difficulty scaling
 * 
 * @see Combat
 * @see Player
 */

#pragma once
#include <vector>

/**
 * @struct Zombie
 * @brief Represents a single zombie entity with position and health
 */
struct Zombie {
    int x;          // X coordinate position
    int y;          // Y coordinate position
    int health;     // Current health points
    
    /**
     * @brief Constructor for a Zombie
     * @param x Initial X coordinate
     * @param y Initial Y coordinate
     * @param health Initial health points
     */
    Zombie(int x, int y, int health);
    
    /**
     * @brief Gets the display character for the zombie
     * @return Character representing the zombie
     */
    char getDisplayChar() const;
};

/**
 * @class ZombieManager
 * @brief Manages a collection of zombies, their spawning, movement, and interactions
 */
class ZombieManager {
private:
    std::vector<Zombie> zombies;   // Collection of active zombies
    int enemyInitHP;               // Initial health points for new zombies
    int enemySpeed;                // Movement speed of zombies
    int enemyMoveCounter;          // Counter for zombie movement timing
    int enemySpawnCounter;         // Counter for zombie spawning timing
    int enemySpawnInterval;        // Time between zombie spawns
    int spawnProbability;          // Probability of spawning a new zombie
    
public:
    /**
     * @brief Constructor for ZombieManager
     * @param difficulty Current game difficulty
     * @param gameLevel Current game level
     */
    ZombieManager(int difficulty, int gameLevel);
    
    /**
     * @brief Updates all zombies' states
     */
    void update();
    
    /**
     * @brief Spawns a new zombie if conditions are met
     */
    void spawnZombie();
    
    /**
     * @brief Moves all zombies according to their behavior
     */
    void moveZombies();
    
    /**
     * @brief Processes collision between a position and zombies
     * @param x X coordinate to check
     * @param y Y coordinate to check
     * @param damage Damage to apply on collision
     * @return Number of zombies hit
     */
    int processCollision(int x, int y, int damage);
    
    /**
     * @brief Gets the number of zombies that have escaped
     * @return Number of escaped zombies
     */
    int getEscapedZombies();
    
    /**
     * @brief Gets the collection of active zombies
     * @return Reference to the vector of zombies
     */
    const std::vector<Zombie>& getZombies() const;
    
    /**
     * @brief Gets the initial health points for new zombies
     * @return Initial health points value
     */
    int getInitHP() const;
}; 