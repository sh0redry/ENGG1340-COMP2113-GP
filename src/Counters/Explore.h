/**
 * @file Explore.h
 * @brief Exploration counter for territory expansion
 * 
 * This file defines the ExploreCounter class, which manages the exploration
 * system in the game. It allows players to explore new territories, with
 * risks and rewards varying based on the number of explorers and difficulty
 * settings.
 * 
 * The ExploreCounter class features:
 * - Territory exploration mechanics
 * - Risk and reward system
 * - Resource discovery
 * - Player resource management
 * - Difficulty-based risk adjustments
 * - Seasonal effects on exploration
 * 
 * @see CounterBase
 * @see Player
 * @see WeekCycle
 * @see Difficulty
 */

#pragma once
#include "CounterBase.h"
#include "../Utils/Random.h"
#include <string>
#include <unordered_map>
#include "../Core/WeekCycle.h"
#include "../Utils/SpecialFunctions.h"

/**
 * @brief Class representing the exploration system in the game
 * 
 * This class handles the exploration mechanics where players can send people
 * to explore and potentially find resources, crops, or new people.
 */
class ExploreCounter : public CounterBase {
private:
    // Pointer to the current ExploreCounter instance
    static ExploreCounter* currentInstance;
    
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

    // Reference to the week cycle system
    WeekCycle& m_weekCycle;

public:
    /**
     * @brief Constructor for ExploreCounter
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle system
     */
    explicit ExploreCounter(Player& player, WeekCycle& weekCycle);

    /**
     * @brief Called when entering the exploration system
     */
    void OnEnter() override;

    /**
     * @brief Main processing loop for exploration
     */
    void Process() override;

    /**
     * @brief Called when exiting the exploration system
     */
    void OnExit() override;

private:
    /**
     * @brief Enum representing possible exploration outcomes
     */
    enum class ExploreResult {
        GOLD_FOUND,        // Found gold during exploration
        CROP_FOUND,        // Found crops during exploration
        PEOPLE_JOINED,     // New people joined the settlement
        PEOPLE_LOST,       // Lost people during exploration
        NOTHING_FOUND,     // No significant findings
    };

    // Configuration table for difficulty levels
    static const std::unordered_map<int, float> DIFFICULTY_CONFIGS;

    // Reward range constants
    static constexpr int MIN_GOLD_REWARD = 10;    // Minimum gold reward
    static constexpr int MAX_GOLD_REWARD = 50;    // Maximum gold reward
    static constexpr int MIN_CROP_REWARD = 10;    // Minimum crop reward
    static constexpr int MAX_CROP_REWARD = 50;    // Maximum crop reward
    static constexpr int MIN_PEOPLE_REWARD = 1;   // Minimum people reward
    static constexpr int MAX_PEOPLE_REWARD = 3;   // Maximum people reward

    /**
     * @brief Execute the core exploration logic
     * @return ExploreResult indicating the outcome of exploration
     */
    ExploreResult executeExplore();
    
    /**
     * @brief Apply the results of exploration to the game state
     * @param result The exploration result
     * @param peopleSent Number of people sent on exploration
     * @param value Reference to store the reward value
     */
    void applyResult(ExploreResult result, int peopleSent, int& value);

    /**
     * @brief Generate result messages for the UI
     * @param result The exploration result
     * @param value The reward value
     * @return Vector of strings containing the result messages
     */
    std::vector<std::string> showResultMessage(ExploreResult result, int value);
    
    /**
     * @brief Get valid input for number of people to send
     * @param max Maximum number of people that can be sent
     * @return Valid number of people to send
     */
    int getValidPeopleInput(int max);
};
