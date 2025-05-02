/**
 * @file Combat.h
 * @brief Combat system for player-zombie interactions
 * 
 * This file defines the Combat class, which manages the core combat mechanics
 * of the game. It handles player movement, shooting, zombie spawning, and
 * collision detection in a grid-based combat system.
 * 
 * The Combat class features:
 * - Player movement and shooting mechanics
 * - Zombie spawning and movement
 * - Collision detection
 * - Game state management
 * - Pause functionality
 * - Real-time display updates
 * 
 * @see Player
 * @see Weapon
 * @see Zombie
 * @see WeekCycle
 */

#pragma once
#include "Weapon.h"
#include "Zombie.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../UI/Animation.h"
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include "../Utils/SpecialFunctions.h"
#include <vector>
#include <chrono>

/**
 * @class Combat
 * @brief Manages the combat system including player movement, weapon usage, and zombie interactions
 */
class Combat {
private:
    // Game board dimensions and display characters
    static const int WIDTH = 25;
    static const int HEIGHT = 25;
    static const char PLAYER_CHAR = 'A';
    static const char BULLET_CHAR = '|';
    
    Player& player;                // Reference to the player object
    Weapon weapon;                 // Player's weapon
    ZombieManager zombieManager;   // Manages zombie spawning and movement
    std::vector<std::pair<int, int>> bullets;  // Active bullets in the game
    
    int playerX;                   // Player's X coordinate
    int playerY;                   // Player's Y coordinate
    int initialHP;                 // Initial health points
    int HP;                        // Current health points
    int gameDuration;              // Total game duration in seconds
    std::chrono::steady_clock::time_point startTime;  // Game start time
    
    // Pause system variables
    bool isPaused;                 // Current pause state
    static Combat* currentInstance; // Current combat instance for pause handling
    std::chrono::steady_clock::time_point pauseStartTime;  // When the game was paused
    std::chrono::steady_clock::duration pausedDuration;    // Total time spent paused
    
    /**
     * @brief Sets up the pause callback function
     */
    void setupPauseCallback();
    
    /**
     * @brief Clears the pause callback function
     */
    void clearPauseCallback();
    
    /**
     * @brief Static callback function to toggle pause state
     */
    static void togglePauseCallback();
    
    /**
     * @brief Toggles the pause state of the game
     */
    void togglePause();
    
    /**
     * @brief Processes user input for player movement and shooting
     */
    void processInput();
    
    /**
     * @brief Updates game state including bullets, zombies, and collisions
     */
    void update();
    
    /**
     * @brief Renders the current game state to the screen
     */
    void draw() const;
    
    /**
     * @brief Checks if the game time has expired
     * @return true if time is up, false otherwise
     */
    bool isTimeUp() const;
    
    /**
     * @brief Calculates remaining game time
     * @return Remaining time in seconds
     */
    int getRemainingTime() const;
    
public:
    /**
     * @brief Constructor for the Combat system
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle object for difficulty scaling
     */
    Combat(Player& player, const WeekCycle& weekCycle);
    
    /**
     * @brief Main game loop
     * @return true if player wins, false if player loses
     */
    bool run();
}; 