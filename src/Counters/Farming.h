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