/**
 * @file Shop.h
 * @brief Shop counter for weapon upgrades
 * 
 * This file defines the ShopCounter class, which manages the weapon upgrade
 * system in the game. It provides functionality for players to upgrade their
 * weapons using gold, with each upgrade increasing the weapon's power and
 * capabilities.
 * 
 * The ShopCounter class features:
 * - Weapon upgrade mechanics
 * - Cost calculation and resource management
 * - Upgrade confirmation system
 * - Weapon information display
 * - Player resource validation
 * 
 * @see CounterBase
 * @see Player
 * @see WeekCycle
 */

#pragma once
#include "CounterBase.h"
#include "../Core/WeekCycle.h"

/**
 * @brief Class representing the shop system in the game
 * 
 * This class handles the shop mechanics where players can purchase weapons
 * and upgrades. The shop system displays weapon information, player resources,
 * and handles the transaction process.
 */
class ShopCounter : public CounterBase {
private:
    /**
     * @brief Get valid input for shop menu selection
     * @return Valid menu selection number
     */
    int GetValidInput();
    
    // Pointer to the current ShopCounter instance
    static ShopCounter* currentInstance;
    
    /**
     * @brief Display weapon power and level information
     */
    void ShowWeaponPowerAndLevelInfo();
    static void ShowWeaponPowerAndLevelInfoCallback();

    /**
     * @brief Display player's current information and resources
     */
    void ShowPlayerInfo();
    static void ShowPlayerInfoCallback();

    /**
     * @brief Display quit message when player wants to exit
     */
    void ShowQuitMessage();
    static void ShowQuitMessageCallback();

    // Reference to the week cycle system
    WeekCycle& m_weekCycle;

public:
    /**
     * @brief Constructor for ShopCounter
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle system
     */
    explicit ShopCounter(Player& player, WeekCycle& weekCycle);

    /**
     * @brief Called when entering the shop system
     */
    void OnEnter() override;

    /**
     * @brief Main processing loop for shop interactions
     */
    void Process() override;

    /**
     * @brief Called when exiting the shop system
     */
    void OnExit() override;
};