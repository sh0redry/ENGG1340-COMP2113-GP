#include "Explore.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../Utils/Random.h"
#include "../Core/Difficulty.h"
#include "../Core/Player.h"
#include "../UI/Animation.h"
#include "../Core/WeekCycle.h"
#include "../Utils/SpecialFunctions.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

// Initialize static member
ExploreCounter* ExploreCounter::currentInstance = nullptr;

/**
 * @brief Constructor for ExploreCounter
 * @param player Reference to the player object
 * @param weekCycle Reference to the week cycle system
 * 
 * Initializes the exploration counter with player and week cycle references,
 * and sets up the current instance pointer.
 */
ExploreCounter::ExploreCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Explore"), m_weekCycle(weekCycle) {
    currentInstance = this;
}

/**
 * @brief Called when entering the exploration system
 * 
 * Sets up key callbacks and displays the initial exploration interface
 * with animated introduction text.
 */
void ExploreCounter::OnEnter() {
    // Set up key callbacks
    setupHKeyCallback();
    setupLKeyCallback(ShowPlayerInfoCallback);
    setupQKeyCallback(ShowQuitMessageCallback);
    
    UI::ShowInterface("ui/Counters/Explore/explore1.txt");
    Animation::TypewriterInBox("Step into this land, and ahead lies the mysterious and uncharted territory,", 50, 16);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("fraught with perils yet hiding endless treasures,", 50, 17);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("where high risks and high rewards go hand in hand ~", 50, 18);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

/**
 * @brief Main processing loop for exploration
 * 
 * Handles the core exploration mechanics including:
 * 1. Getting valid input for number of people to send
 * 2. Executing exploration
 * 3. Applying results
 * 4. Displaying results with animations
 */
void ExploreCounter::Process() {
    int peopleSent = getValidPeopleInput(m_player.getAvailablePeople());
    
    if (peopleSent > 0) {
        m_player.assignWorkers(0, 0, 0, 0, peopleSent);
        ExploreResult result = executeExplore();
        int value = 0;
        applyResult(result, peopleSent, value);
        
        UI::WaitForEnter();
        // Display exploration in progress message with typewriter effect
        UI::ShowInterface("ui/Counters/Explore/explore1.txt");
        Animation::TypewriterInBox("Your guys are exploring the land, which is full of perils and treasures!", 50, 16);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Will they return with treasures or in pieces?", 50, 17);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Let's pray for them ......", 50, 18);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // Display results with typewriter effect
        std::vector<std::string> messages = showResultMessage(result, value);
        UI::ShowInterface("ui/Counters/Explore/explore2.txt");
        UI::DisplayCenterText(messages[0], 23);
        UI::DisplayCenterText(messages[1], 25);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
}

/**
 * @brief Called when exiting the exploration system
 * 
 * Cleans up by removing all key callbacks and resetting the current instance pointer.
 */
void ExploreCounter::OnExit() {
    // Clear all callbacks
    clearHKeyCallback();
    clearLKeyCallback();
    clearQKeyCallback();
    // Clear current instance
    currentInstance = nullptr;
}

// --- Private method implementations ---

/**
 * @brief Execute the core exploration logic
 * @return ExploreResult indicating the outcome of exploration
 * 
 * Determines the exploration outcome based on:
 * 1. Difficulty-based loss probability
 * 2. Random distribution of rewards (gold:crops:people:nothing = 2:2:2:1)
 */
ExploreCounter::ExploreResult ExploreCounter::executeExplore() {
    // Get difficulty configuration
    const float lossProbability = Difficulty::GetConfig(m_player.getStringDifficulty()).exploreRisk;

    // Check if people are lost
    if (Random::Chance(lossProbability)) {
        return ExploreResult::PEOPLE_LOST;
    }

    // If not lost, distribute probability according to 2:2:2:1 ratio
    float remainingProbability = (1.0f - lossProbability) / 7.0f; // Total parts = 2+2+2+1 = 7
    float randomValue = Random::RangeReal(0.0f, 1.0f);

    if (randomValue < 2 * remainingProbability) {
        return ExploreResult::GOLD_FOUND;
    } else if (randomValue < 4 * remainingProbability) {
        return ExploreResult::CROP_FOUND;
    } else if (randomValue < 6 * remainingProbability) {
        return ExploreResult::PEOPLE_JOINED;
    } else {
        return ExploreResult::NOTHING_FOUND;
    }
}

/**
 * @brief Apply the results of exploration to the game state
 * @param result The exploration result
 * @param peopleSent Number of people sent on exploration
 * @param value Reference to store the reward value
 * 
 * Updates player resources based on exploration outcome:
 * - Gold found: Adds gold to player
 * - Crops found: Adds crops to player
 * - People joined: Adds new people
 * - People lost: Removes people
 * - Nothing found: No changes
 */
void ExploreCounter::applyResult(ExploreResult result, int peopleSent, int& value) {
    switch (result) {
        case ExploreResult::GOLD_FOUND:
            value = Random::Range(MIN_GOLD_REWARD, MAX_GOLD_REWARD) * peopleSent;
            m_player.addGold(value);
            break;
            
        case ExploreResult::CROP_FOUND:
            value = Random::Range(MIN_CROP_REWARD, MAX_CROP_REWARD) * peopleSent;
            m_player.addCrop(value);
            break;
            
        case ExploreResult::PEOPLE_JOINED:
            value = Random::Range(MIN_PEOPLE_REWARD, MAX_PEOPLE_REWARD);
            m_player.addPeople(value);
            break;
            
        case ExploreResult::PEOPLE_LOST:
            value = peopleSent; // Lost all exploration members
            m_player.addPeople(-value);
            break;
            
        case ExploreResult::NOTHING_FOUND:
            value = 0;
            break;
            
        default: break;
    }
}

/**
 * @brief Generate result messages for the UI
 * @param result The exploration result
 * @param value The reward value
 * @return Vector of strings containing the result messages
 * 
 * Creates appropriate messages to display based on the exploration outcome.
 */
std::vector<std::string> ExploreCounter::showResultMessage(ExploreResult result, int value) {
    std::vector<std::string> messages;
    
    switch (result) {
        case ExploreResult::GOLD_FOUND:
            messages.push_back("Exploration Successful! Congratulations!");
            messages.push_back("Gold Found: " + std::to_string(value));
            break;
        case ExploreResult::CROP_FOUND:
            messages.push_back("Exploration Successful! Congratulations!");
            messages.push_back("Food Found: " + std::to_string(value));
            break;
        case ExploreResult::PEOPLE_JOINED:
            messages.push_back("Congratulations! Survivors Found!");
            messages.push_back("New Members: " + std::to_string(value));
            break;
        case ExploreResult::PEOPLE_LOST:
            messages.push_back("OHHH NO!!! They sacrificed!");
            messages.push_back("Members Lost: " + std::to_string(value));
            break;
        case ExploreResult::NOTHING_FOUND:
            messages.push_back("Exploration Completed.");
            messages.push_back("Nothing was found.");
            break;
        default:
            break;
    }
    return messages;
}

/**
 * @brief Static callback for showing player information
 * 
 * Called when 'L' key is pressed to display player stats.
 */
void ExploreCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

/**
 * @brief Display player information and restore exploration interface
 * 
 * Shows player stats and resources, then restores the exploration interface
 * with appropriate prompts and instructions.
 */
void ExploreCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Explore/explore2.txt");
    UI::DisplayCenterText("Type in the number of people you want to assign to this land!", 22);
    UI::DisplayCenterText("Think twice before you decide!", 23);
    UI::DisplayCenterText("The rewards are tempting, but you could die for it.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign people to explore (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 26);
}

/**
 * @brief Get valid input for number of people to send
 * @param max Maximum number of people that can be sent
 * @return Valid number of people to send
 * 
 * Handles input validation and user interface for selecting number of people
 * to send on exploration. Includes error handling and feedback.
 */
int ExploreCounter::getValidPeopleInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Explore/explore2.txt");
        UI::DisplayCenterText("Type in the number of people you want to assign to this land!", 22);
        UI::DisplayCenterText("Think twice before you decide!", 23);
        UI::DisplayCenterText("The rewards are tempting, but you could die for it.", 24);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
        UI::DisplayCenterText("Assign people to explore (0-" + std::to_string(max) + "): ", 26);
    
        int input = Terminal::GetInstance().GetInteger();
    
        if (input >= 0 && input <= max) {
            UI::DisplayCenterText("Successfully assigned " + std::to_string(input)  + " people to explore!", 27);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return input;
        }
        else {
            UI::DisplayCenterText("Invalid input! Must be between 0 and " + std::to_string(max) + "!", 27);
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
void ExploreCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

/**
 * @brief Display quit message and restore exploration interface
 * 
 * Shows quit confirmation message and restores the exploration interface
 * with appropriate prompts and instructions.
 */
void ExploreCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Explore/explore2.txt");
    UI::DisplayCenterText("Type in the number of people you want to assign to this land!", 22);
    UI::DisplayCenterText("Think twice before you decide!", 23);
    UI::DisplayCenterText("The rewards are tempting, but you could die for it.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign people to explore (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 26);
}
