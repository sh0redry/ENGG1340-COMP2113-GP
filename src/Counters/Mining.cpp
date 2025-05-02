/**
 * @file Mining.cpp
 * @brief Implementation of the mining counter system
 * @details Handles mining operations including worker assignment,
 *          gold production, and resource management for mining
 *          activities.
 */
#include "Mining.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include "../UI/Animation.h"
#include "../Core/Difficulty.h"
#include "../Utils/SpecialFunctions.h"
#include <iostream>
#include <thread>
#include <chrono>

// Initialize static member
MiningCounter* MiningCounter::currentInstance = nullptr;

/**
 * @brief Constructor for MiningCounter
 * @param player Reference to the player object
 * @param weekCycle Reference to the week cycle system
 * 
 * Initializes the mining counter with player and week cycle references,
 * and sets up the current instance pointer.
 */
MiningCounter::MiningCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Mining"), m_weekCycle(weekCycle) {
    currentInstance = this;
}

/**
 * @brief Called when entering the mining system
 * 
 * Sets up key callbacks and displays the initial mining interface
 * with animated introduction text.
 */
void MiningCounter::OnEnter() {
    // Set up key callbacks
    setupHKeyCallback();
    setupLKeyCallback(ShowPlayerInfoCallback);
    setupQKeyCallback(ShowQuitMessageCallback);
    
    UI::ShowInterface("ui/Counters/Mining/mining1.txt");
    Animation::TypewriterInBox("Oh! My dear owner, ", 50, 23);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("I've been waiting for you here for a long time!", 50, 24);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("The gold mine seems to have brightened up with your arrival.", 50, 25);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("I'll lead the way for you right away.", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

/**
 * @brief Main processing loop for mining
 * 
 * Handles the core mining mechanics including:
 * 1. Getting valid input for number of miners to assign
 * 2. Calculating gold yield based on difficulty
 * 3. Updating player resources
 * 4. Displaying results with animations
 */
void MiningCounter::Process() {
    const int yield = Difficulty::GetConfig(m_player.getStringDifficulty()).goldYield;
    int workers = GetValidInput(m_player.getAvailablePeople());

    if (workers > 0) {
        m_player.addGold(yield * workers);
        m_player.assignWorkers(0, workers, 0, 0, 0);
        
        UI::WaitForEnter();
        // Display mining in progress message with typewriter effect
        UI::ShowInterface("ui/Counters/Mining/mining3.txt");
        Animation::TypewriterInBox("Your guys are working hard to get more golds!", 50, 23);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // Display results with typewriter effect
        UI::ShowInterface("ui/Counters/Mining/mining2.txt");
        UI::DisplayCenterText("You got " + std::to_string(yield * workers) + " golds!", 24);
        UI::DisplayCenterText("You can use the golds to upgrade your weapons and defend yourself against the zombies!", 26);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
}

/**
 * @brief Called when exiting the mining system
 * 
 * Cleans up by removing all key callbacks and resetting the current instance pointer.
 */
void MiningCounter::OnExit() {
    // Clear all callbacks
    clearHKeyCallback();
    clearLKeyCallback();
    clearQKeyCallback();
    // Clear current instance
    currentInstance = nullptr;
}

/**
 * @brief Static callback for showing player information
 * 
 * Called when 'L' key is pressed to display player stats.
 */
void MiningCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

/**
 * @brief Display player information and restore mining interface
 * 
 * Shows player stats and resources, then restores the mining interface
 * with appropriate prompts and instructions.
 */
void MiningCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Mining/mining2.txt");
    UI::DisplayCenterText("Here is a mine, the minerals you get from here", 24);
    UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign miners (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 27);
}

/**
 * @brief Get valid input for number of miners to assign
 * @param max Maximum number of miners that can be assigned
 * @return Valid number of miners to assign
 * 
 * Handles input validation and user interface for selecting number of miners
 * to assign. Includes error handling and feedback.
 */
int MiningCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Mining/mining2.txt");
        UI::DisplayCenterText("Here is a gold mine, and the gold you get from here can be used to upgrade your weapons", 24);
        UI::DisplayCenterText("to better defend yourself against the zombies.", 25);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
        UI::DisplayCenterText("Assign miners (0-" + std::to_string(max) + "): ", 27);
    
        int input = Terminal::GetInstance().GetInteger();
    
        if (input >= 0 && input <= max) {
            UI::DisplayCenterText("Successfully assigned " + std::to_string(input) + " miners!", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return input;
        }
        else {
            UI::DisplayCenterText("Invalid input! Must be between 0 and " + std::to_string(max) + "!", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
        }
    }
}

/**
 * @brief Static callback for showing quit message
 * 
 * Called when 'Q' key is pressed to display quit confirmation.
 */
void MiningCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

/**
 * @brief Display quit message and restore mining interface
 * 
 * Shows quit confirmation message and restores the mining interface
 * with appropriate prompts and instructions.
 */
void MiningCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Mining/mining2.txt");
    UI::DisplayCenterText("Here is a mine, the minerals you get from here", 24);
    UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign miners (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 27);
}