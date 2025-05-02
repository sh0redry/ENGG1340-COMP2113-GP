/**
 * @file Mining.h
 * @brief Mining counter for gold production
 * 
 * This file defines the MiningCounter class, which manages the gold mining
 * system in the game. It allows players to assign workers to mining activities,
 * generating gold based on the number of miners and difficulty settings.
 * 
 * The MiningCounter class features:
 * - Gold production mechanics
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
 * @brief Class representing the mining system in the game
 * 
 * This class handles the mining mechanics where players can assign people
 * to mine and collect resources. The mining system is tied to the week cycle
 * for potential seasonal effects on mining efficiency.
 */
class MiningCounter : public CounterBase {
private:
    /**
     * @brief Get valid input for number of people to assign
     * @param max Maximum number of people that can be assigned
     * @return Valid number of people to assign
     */
    int GetValidInput(int max);
    
    // Pointer to the current MiningCounter instance
    static MiningCounter* currentInstance;
    
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
     * @brief Constructor for MiningCounter
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle system
     */
    explicit MiningCounter(Player& player, WeekCycle& weekCycle);

    /**
     * @brief Called when entering the mining system
     */
    void OnEnter() override;

    /**
     * @brief Main processing loop for mining
     */
    void Process() override;

    /**
     * @brief Called when exiting the mining system
     */
    void OnExit() override;
};