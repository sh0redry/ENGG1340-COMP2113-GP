/**
 * @file Weapon.h
 * @brief Weapon system for player combat
 * 
 * This file defines the Weapon class, which manages the player's weapon
 * properties and upgrades. It implements a progression system with multiple
 * levels, damage scaling, and different shot patterns.
 * 
 * The Weapon class features:
 * - Multiple weapon levels (1-10)
 * - Progressive damage scaling
 * - Multiple shot patterns
 * - Upgrade system
 * 
 * @see Combat
 */

#pragma once

/**
 * @class Weapon
 * @brief Manages weapon properties including damage, level, and multiple shot capability
 */
class Weapon {
private:
    int level;      // Current weapon level
    int damage;     // Current weapon damage
    int multiple;   // Number of simultaneous shots

    // Weapon damage configuration for each level
    static const int weaponDamage[10];
    // Weapon multiple shot configuration for each level
    static const int weaponMultiple[10];

public:
    /**
     * @brief Constructor for the Weapon class
     * @param level Initial weapon level (default: 1)
     */
    Weapon(int level = 1);
    
    /**
     * @brief Upgrades the weapon to the next level
     */
    void upgrade();
    
    /**
     * @brief Checks if the weapon can be upgraded further
     * @return true if upgrade is possible, false otherwise
     */
    bool canUpgrade() const;
    
    /**
     * @brief Gets the current weapon damage
     * @return Current damage value
     */
    int getDamage() const;
    
    /**
     * @brief Gets the number of simultaneous shots
     * @return Number of multiple shots
     */
    int getMultiple() const;
    
    /**
     * @brief Gets the current weapon level
     * @return Current level
     */
    int getLevel() const;
    
    /**
     * @brief Gets the maximum possible weapon level
     * @return Maximum level value
     */
    static int getMaxLevel();
}; 