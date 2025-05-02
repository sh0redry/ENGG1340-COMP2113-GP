/**
 * @file Farming.h
 * @brief Farming counter for crop production
 * 
 * This file defines the FarmingCounter class, which manages the crop production
 * system in the game. It allows players to assign workers to farming activities,
 * generating crops based on the number of farmers and difficulty settings.
 * 
 * The FarmingCounter class features:
 * - Crop production mechanics
 * - Worker assignment system
 * - Resource yield calculation
 * - Player resource management
 * - Difficulty-based yield adjustments
 * 
 * @see CounterBase
 * @see Player
 * @see WeekCycle
 * @see Difficulty
 */

#pragma once
#include "CounterBase.h"
#include "../Core/WeekCycle.h"

/**
 * @brief Class representing the farming system in the game
 * 
 * This class handles the farming mechanics where players can assign people
 * to farm and produce crops. The farming system is tied to the week cycle
 * for seasonal effects and crop growth.
 */
class FarmingCounter : public CounterBase {
private:
    /**
     * @brief Get valid input for number of people to assign
     * @param max Maximum number of people that can be assigned
     * @return Valid number of people to assign
     */
    int GetValidInput(int max);
    
    // Pointer to the current FarmingCounter instance
    static FarmingCounter* currentInstance;
    
    /**
     * @brief Display player's current information
     */
    void ShowPlayerInfo();
    static void ShowPlayerInfoCallback();

    /**
     * @brief Display quit message when player wants to exit
     */
    void ShowQuitMessage();
    static void ShowQuitMessageCallback();

    // Reference to the week cycle system for seasonal effects
    WeekCycle& m_weekCycle;

public:
    /**
     * @brief Constructor for FarmingCounter
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle system
     */
    explicit FarmingCounter(Player& player, WeekCycle& weekCycle);

    /**
     * @brief Called when entering the farming system
     */
    void OnEnter() override;

    /**
     * @brief Main processing loop for farming
     */
    void Process() override;

    /**
     * @brief Called when exiting the farming system
     */
    void OnExit() override;
};