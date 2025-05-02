#pragma once
#include "CounterBase.h"
#include "../Core/WeekCycle.h"

/**
 * @brief Class representing the recruitment system in the game
 * 
 * This class handles the recruitment mechanics where players can hire new
 * people to join their settlement. The recruitment system includes cost
 * calculations and input validation for the recruitment process.
 */
class RecruitCounter : public CounterBase {
private:
    // Base cost for recruitment (can be modified by game mechanics)
    static constexpr int BASE_COST = 0;
    // Cost per person to recruit
    static constexpr int COST_PER_MEMBER = 30;
    
    // Pointer to the current RecruitCounter instance
    static RecruitCounter* currentInstance;
    
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

    /**
     * @brief Enum representing the current input state of the recruitment process
     */
    enum class InputState {
        WAITING_YN,     // Waiting for yes/no input
        WAITING_NUMBER  // Waiting for number input
    };
    InputState m_currentState;

    /**
     * @brief Get valid input for number of people to recruit
     * @param max Maximum number of people that can be recruited
     * @return Valid number of people to recruit
     */
    int GetValidInput(int max);

    /**
     * @brief Calculate the maximum number of people that can be recruited
     * based on available resources and other constraints
     * @return Maximum number of people that can be recruited
     */
    int calculateMaxRecruits() const;

public:
    /**
     * @brief Constructor for RecruitCounter
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle system
     */
    explicit RecruitCounter(Player& player, WeekCycle& weekCycle);

    /**
     * @brief Called when entering the recruitment system
     */
    void OnEnter() override;

    /**
     * @brief Main processing loop for recruitment
     */
    void Process() override;

    /**
     * @brief Called when exiting the recruitment system
     */
    void OnExit() override;
};