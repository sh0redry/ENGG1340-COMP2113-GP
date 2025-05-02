#include "Recruit.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../UI/Animation.h"
#include "../Utils/SpecialFunctions.h"

// Initialize static member
RecruitCounter* RecruitCounter::currentInstance = nullptr;

/**
 * @brief Constructor for RecruitCounter
 * @param player Reference to the player object
 * @param weekCycle Reference to the week cycle system
 * 
 * Initializes the recruitment counter with player and week cycle references,
 * sets up the current instance pointer, and initializes the input state.
 */
RecruitCounter::RecruitCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Recruit"), m_weekCycle(weekCycle), m_currentState(InputState::WAITING_YN) {
    currentInstance = this;
}

/**
 * @brief Called when entering the recruitment system
 * 
 * Sets up key callbacks and displays the initial recruitment interface
 * with animated introduction text.
 */
void RecruitCounter::OnEnter() {
    // Set up key callbacks
    setupHKeyCallback();
    setupLKeyCallback(ShowPlayerInfoCallback);
    setupQKeyCallback(ShowQuitMessageCallback);
    
    // Display introduction with typewriter effect
    UI::ShowInterface("ui/Counters/Recruit/recruit1.txt");
    Animation::TypewriterInBox("Hey! Come to hire someone, huh?", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("Is the pay sweet enough?", 50, 27);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("I'll pick some capable ones for you.", 50, 28);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

/**
 * @brief Main processing loop for recruitment
 * 
 * Handles the core recruitment mechanics including:
 * 1. Checking if player has enough resources
 * 2. Getting valid input for number of recruits
 * 3. Processing recruitment and updating resources
 * 4. Displaying results with animations
 */
void RecruitCounter::Process() {
    int maxRecruits = calculateMaxRecruits();
    if (maxRecruits <= 0) {
        UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
        UI::DisplayCenterText("No enough crops to recruit new members!", 25);
        UI::DisplayCenterText("Minimum required: " + std::to_string(BASE_COST + COST_PER_MEMBER) + " crops", 27);
        UI::DisplayCenterText("Current crops: " + std::to_string(m_player.getCrop()) + " crops", 28);
        UI::WaitForEnter("Press Enter to return to home...");
        return;
    }

    int recruits = GetValidInput(maxRecruits);
    if (recruits > 0) {
        int totalCost = BASE_COST + COST_PER_MEMBER * recruits;
        m_player.addCrop(-totalCost);
        m_player.addPeople(recruits);
        m_player.assignWorkers(0, 0, 1, 0, 0);
        
        UI::WaitForEnter();
        UI::ShowInterface("ui/Counters/Recruit/recruit3.txt");
        Animation::TypewriterInBox("Your soldiers are ready for fight!", 50, 25);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // Display results with typewriter effect
        UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
        UI::DisplayCenterText("Now you have " + std::to_string(recruits) + " more members!", 25);
        UI::DisplayCenterText("Consumed crops: " + std::to_string(totalCost), 27);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
}

/**
 * @brief Static callback for showing player information
 * 
 * Called when 'L' key is pressed to display player stats.
 */
void RecruitCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

/**
 * @brief Display player information and restore recruitment interface
 * 
 * Shows player stats and resources, then restores the recruitment interface
 * with appropriate prompts and instructions based on current input state.
 */
void RecruitCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // Restore interface based on current state
    UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
    UI::DisplayCenterText("This is the recruiting office. You can use crops to recruit new members.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 32);
    
    if (m_currentState == InputState::WAITING_YN) {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] ", 26);
    } else {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] y", 26);
        UI::DisplayCenterText("Recruit how many members? (0-" + std::to_string(calculateMaxRecruits()) + "): ", 28);
    }
}

/**
 * @brief Get valid input for recruitment process
 * @param max Maximum number of recruits possible
 * @return Number of recruits to hire
 * 
 * Handles the two-step input process:
 * 1. Yes/No confirmation for recruitment
 * 2. Number of recruits to hire
 * Includes error handling and feedback.
 */
int RecruitCounter::GetValidInput(int max) {
    while (true) {
        m_currentState = InputState::WAITING_YN;
        UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
        UI::DisplayCenterText("This is the recruiting office. You can use crops to recruit new members.", 24);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 32);
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] ", 26);
        char yn = Terminal::GetInstance().GetYN();
        if (yn != 'y' && yn != 'Y' && yn != 'n' && yn != 'N'){
            UI::DisplayCenterText("Invalid input! Please enter y or n", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
            continue;
        }else if (yn == 'y' || yn == 'Y'){
            while (true) {
                m_currentState = InputState::WAITING_NUMBER;
                // The first box in this txt file contains a 'y'
                UI::DisplayCenterText("Recruit how many members? (0-" + std::to_string(max) + "): ", 28);

                int input = Terminal::GetInstance().GetInteger();

                if (input >= 0 && input <= max) {
                    UI::DisplayCenterText("Successfully recruited " + std::to_string(input) + " members!", 29);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    return input;
                } else {
                    UI::DisplayCenterText("Must be between 0 and " + std::to_string(max) + "!", 29);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    UI::WaitForEnter("Press Enter to try again...");
                }
           }
        }else if (yn == 'n' || yn == 'N'){
            UI::DisplayCenterText("You will not recruit any member.", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return 0;
        }
    }
}

/**
 * @brief Calculate maximum number of possible recruits
 * @return Maximum number of recruits based on available crops
 * 
 * Calculates how many recruits the player can afford based on:
 * - Available crops
 * - Base cost
 * - Cost per member
 */
int RecruitCounter::calculateMaxRecruits() const {
    return std::max(0, (m_player.getCrop() - BASE_COST) / COST_PER_MEMBER);
}

/**
 * @brief Static callback for showing quit message
 * 
 * Called when 'Q' key is pressed to display quit confirmation.
 */
void RecruitCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

/**
 * @brief Display quit message and restore recruitment interface
 * 
 * Shows quit confirmation message and restores the recruitment interface
 * with appropriate prompts and instructions based on current input state.
 */
void RecruitCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
    UI::DisplayCenterText("This is the recruiting office. You can use crops to recruit new members.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 32);
    
    if (m_currentState == InputState::WAITING_YN) {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] ", 26);
    } else {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] y", 26);
        UI::DisplayCenterText("Recruit how many members? (0-" + std::to_string(calculateMaxRecruits()) + "): ", 28);
    }
}

/**
 * @brief Called when exiting the recruitment system
 * 
 * Cleans up by removing all key callbacks and resetting the current instance pointer.
 */
void RecruitCounter::OnExit() {
    // Clear all callbacks
    clearHKeyCallback();
    clearLKeyCallback();
    clearQKeyCallback();
    // Clear current instance
    currentInstance = nullptr;
}