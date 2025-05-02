#include "Farming.h"
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
FarmingCounter* FarmingCounter::currentInstance = nullptr;

/**
 * @brief Constructor for FarmingCounter
 * @param player Reference to the player object
 * @param weekCycle Reference to the week cycle system
 * 
 * Initializes the farming counter with player and week cycle references,
 * and sets up the current instance pointer.
 */
FarmingCounter::FarmingCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Farming"), m_weekCycle(weekCycle) {
    currentInstance = this;
}

/**
 * @brief Called when entering the farming system
 * 
 * Sets up key callbacks and displays the initial farming interface
 * with animated introduction text.
 */
void FarmingCounter::OnEnter() {
    // Set up key callbacks
    setupHKeyCallback();
    setupLKeyCallback(ShowPlayerInfoCallback);
    setupQKeyCallback(ShowQuitMessageCallback);
    
    UI::ShowInterface("ui/Counters/Farming/farming1.txt");
    Animation::TypewriterInBox("Welcome to my farm, guys.", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("It's time to work hard for your survival.", 50, 27);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

/**
 * @brief Main processing loop for farming
 * 
 * Handles the core farming mechanics including:
 * 1. Getting valid input for number of farmers to assign
 * 2. Calculating crop yield based on difficulty
 * 3. Updating player resources
 * 4. Displaying results with animations
 */
void FarmingCounter::Process() {
    const int yield = Difficulty::GetConfig(m_player.getStringDifficulty()).cropYield;
    int workers = GetValidInput(m_player.getAvailablePeople());

    if (workers > 0) {
        m_player.addCrop(yield * workers);
        m_player.assignWorkers(workers, 0, 0, 0, 0);
        
        UI::WaitForEnter();
        // Display farming in progress message with typewriter effect
        UI::ShowInterface("ui/Counters/Farming/farming3.txt");
        Animation::TypewriterInBox("Your guys are working hard to harvest more crops!", 50, 31);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // Display results with typewriter effect
        UI::ShowInterface("ui/Counters/Farming/farming2.txt");
        UI::DisplayCenterText("You got " + std::to_string(yield * workers) + " crops!", 25);
        UI::DisplayCenterText("You can use the crops to recruit new members and grow your team!", 27);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
}

/**
 * @brief Called when exiting the farming system
 * 
 * Cleans up by removing all key callbacks and resetting the current instance pointer.
 */
void FarmingCounter::OnExit() {
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
void FarmingCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

/**
 * @brief Display player information and restore farming interface
 * 
 * Shows player stats and resources, then restores the farming interface
 * with appropriate prompts and instructions.
 */
void FarmingCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Farming/farming2.txt");
    UI::DisplayCenterText("Here is a farm, the crops you get from here", 24);
    UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign farmers (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 27);
}

/**
 * @brief Static callback for showing quit message
 * 
 * Called when 'Q' key is pressed to display quit confirmation.
 */
void FarmingCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

/**
 * @brief Display quit message and restore farming interface
 * 
 * Shows quit confirmation message and restores the farming interface
 * with appropriate prompts and instructions.
 */
void FarmingCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Farming/farming2.txt");
    UI::DisplayCenterText("Here is a farm, the crops you get from here", 24);
    UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign farmers (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 27);
}

/**
 * @brief Get valid input for number of farmers to assign
 * @param max Maximum number of farmers that can be assigned
 * @return Valid number of farmers to assign
 * 
 * Handles input validation and user interface for selecting number of farmers
 * to assign. Includes error handling and feedback.
 */
int FarmingCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Farming/farming2.txt");
        UI::DisplayCenterText("Here is a farm, the crops you get from here", 24);
        UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
        UI::DisplayCenterText("Assign farmers (0-" + std::to_string(max) + "): ", 27);
    
        int input = Terminal::GetInstance().GetInteger();
    
        if (input >= 0 && input <= max) {
            UI::DisplayCenterText("Successfully assigned " + std::to_string(input) + " farmers!", 28);
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

