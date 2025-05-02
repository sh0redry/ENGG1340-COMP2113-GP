/**
 * @file Weapon.cpp
 * @brief Implementation of the weapon system
 * @details Manages weapon properties, upgrade mechanics, and combat
 *          effectiveness calculations for player defense.
 */

#include "Weapon.h"

// Weapon damage configuration for each level
const int Weapon::weaponDamage[10] = {20, 30, 40, 30, 35, 40, 50, 40, 50, 60};
// Weapon multiple shot configuration for each level
const int Weapon::weaponMultiple[10] = {1, 1, 1, 3, 3, 3, 3, 5, 5, 5};

/**
 * @brief Constructor for the Weapon class
 * @param level Initial weapon level (default: 1)
 */
Weapon::Weapon(int level) : level(level) {
    damage = weaponDamage[level - 1];
    multiple = weaponMultiple[level - 1];
}

/**
 * @brief Upgrades the weapon to the next level
 * Updates damage and multiple shot values based on the new level
 */
void Weapon::upgrade() {
    if (canUpgrade()) {
        level++;
        damage = weaponDamage[level - 1];
        multiple = weaponMultiple[level - 1];
    }
}

/**
 * @brief Checks if the weapon can be upgraded further
 * @return true if upgrade is possible, false otherwise
 */
bool Weapon::canUpgrade() const {
    return level < getMaxLevel();
}

/**
 * @brief Gets the current weapon damage
 * @return Current damage value
 */
int Weapon::getDamage() const {
    return damage;
}

/**
 * @brief Gets the number of simultaneous shots
 * @return Number of multiple shots
 */
int Weapon::getMultiple() const {
    return multiple;
}

/**
 * @brief Gets the current weapon level
 * @return Current level
 */
int Weapon::getLevel() const {
    return level;
}

/**
 * @brief Gets the maximum possible weapon level
 * @return Maximum level value (10)
 */
int Weapon::getMaxLevel() {
    return 10;
} 