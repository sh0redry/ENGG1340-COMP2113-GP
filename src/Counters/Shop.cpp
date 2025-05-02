/**
 * @file Shop.cpp
 * @brief Implementation of the shop counter system
 * @details Handles the shop interface, weapon upgrades, resource
 *          management, and player interactions for purchasing
 *          improvements.
 */
#include "Shop.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../Core/Player.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../UI/Animation.h"
#include "../Utils/SpecialFunctions.h"

// Initialize static member
ShopCounter* ShopCounter::currentInstance = nullptr;

/**
 * @brief Constructor for ShopCounter
 * @param player Reference to the player object
 * @param weekCycle Reference to the week cycle system
 * 
 * Initializes the shop counter with player and week cycle references.
 */
ShopCounter::ShopCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Shop"), m_weekCycle(weekCycle) {}

/**
 * @brief Called when entering the shop system
 * 
 * Sets up key callbacks and displays the initial shop interface
 * with animated introduction text.
 */
void ShopCounter::OnEnter() {
    // Set up current instance
    currentInstance = this;
    // Set up key callbacks
    setupHKeyCallback();
    setupWKeyCallback(ShowWeaponPowerAndLevelInfoCallback);
    setupLKeyCallback(ShowPlayerInfoCallback);
    setupQKeyCallback(ShowQuitMessageCallback);
    
    UI::ShowInterface("ui/Counters/Shop/shop1.txt");
    Animation::TypewriterInBox("Yo, what's up? Wanna upgrade your weapon?", 50, 24);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("Gotta see if you've got the skills and the dough.", 50, 25);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("That'll cost ya quite a lot of gold per level.", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("If you can't afford, don't waste my smoke time.", 50, 27);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

/**
 * @brief Main processing loop for weapon upgrades
 * 
 * Handles the core shop mechanics including:
 * 1. Checking weapon level and upgrade costs
 * 2. Validating player resources
 * 3. Processing upgrades
 * 4. Displaying results with animations
 */
void ShopCounter::Process() {
    int currentLevel = m_player.getWeaponLevel();
    int cost = Player::WEAPON_UPGRADE_COST.at(currentLevel);

    if (currentLevel >= 10) {
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Have reached the maximum level! ", 24);
        UI::DisplayCenterText("Current level: " + std::to_string(m_player.getWeaponLevel()), 26);
        UI::WaitForEnter("Press Enter to return to home...");
        return;
    } else if (m_player.getGold() < cost) {
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Not enough gold to upgrade your weapon! ", 24);
        UI::DisplayCenterText("Cost: " + std::to_string(cost) + " gold", 26);
        UI::DisplayCenterText("Current gold: " + std::to_string(m_player.getGold()), 27);
        UI::WaitForEnter("Press Enter to return to home...");
        return;
    }

    if (GetValidInput()) {
        m_player.upgradeWeapon();
        m_player.assignWorkers(0, 0, 0, 1, 0);
        UI::WaitForEnter();
        // Display upgrade in progress message with typewriter effect
        UI::ShowInterface("ui/Counters/Shop/shop3.txt");
        Animation::TypewriterInBox("Um, what a beautiful and powerful weapon!", 50, 24);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("It can easily destroy a whole zombie horde!", 50, 25);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // Display results with typewriter effect
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Current level: " + std::to_string(m_player.getWeaponLevel()), 25);
        UI::DisplayCenterText("Cost: " + std::to_string(cost) + " gold", 27);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    UI::WaitForEnter("Press Enter to return to home...");
}

/**
 * @brief Get valid input for weapon upgrade confirmation
 * @return 1 if upgrade confirmed, 0 if cancelled
 * 
 * Handles the yes/no confirmation process for weapon upgrades,
 * including input validation and user feedback.
 */
int ShopCounter::GetValidInput() {
    while (true) {
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Here you can upgrade your weapon to increase its power and fire count.", 24);
        UI::DisplayCenterText("The better your weapon, the longer you can survive!", 25);
        UI::DisplayCenterText("You can only upgrade one level at a time.", 31);
        UI::DisplayCenterText("W: show weapon information | H: return to home | L: show information | Q: quit", 32);
        UI::DisplayCenterText("Do you want to assign one of your workers to upgrade your weapon? [y/n] ", 27);
        char yn = Terminal::GetInstance().GetYN();
        if (yn != 'y' && yn != 'Y' && yn != 'n' && yn != 'N'){
            UI::DisplayCenterText("Invalid input! Please enter y or n", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
            continue;
        }else if (yn == 'y' || yn == 'Y') {
            UI::DisplayCenterText("Successfully upgraded the weapon!", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return 1;
        }else if (yn == 'n' || yn == 'N') {
            UI::DisplayCenterText("You will not upgrade the weapon.", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return 0;
        }
    }
}

/**
 * @brief Static callback for showing weapon information
 * 
 * Called when 'W' key is pressed to display weapon stats.
 */
void ShopCounter::ShowWeaponPowerAndLevelInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowWeaponPowerAndLevelInfo();
    }
}

/**
 * @brief Display weapon information and restore shop interface
 * 
 * Shows weapon stats and upgrade information, then restores the shop interface
 * with appropriate prompts and instructions.
 */
void ShopCounter::ShowWeaponPowerAndLevelInfo() {
    SpecialFunctions::showWeaponInfo();
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Shop/shop2.txt");
    UI::DisplayCenterText("Here you can upgrade your weapon to increase its power and fire count.", 24);
    UI::DisplayCenterText("The better your weapon, the longer you can survive!", 25);
    UI::DisplayCenterText("You can only upgrade one level at a time.", 31);
    UI::DisplayCenterText("W: show weapon information | H: return to home | L: show information | Q: quit", 32);
    UI::DisplayCenterText("Do you want to assign one of your workers to upgrade your weapon? [y/n] ", 27);
}

/**
 * @brief Static callback for showing player information
 * 
 * Called when 'L' key is pressed to display player stats.
 */
void ShopCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

/**
 * @brief Display player information and restore shop interface
 * 
 * Shows player stats and resources, then restores the shop interface
 * with appropriate prompts and instructions.
 */
void ShopCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Shop/shop2.txt");
    UI::DisplayCenterText("Here you can upgrade your weapon to increase its power and fire count.", 24);
    UI::DisplayCenterText("The better your weapon, the longer you can survive!", 25);
    UI::DisplayCenterText("You can only upgrade one level at a time.", 31);
    UI::DisplayCenterText("W: show weapon information | H: return to home | L: show information | Q: quit", 32);
    UI::DisplayCenterText("Do you want to assign one of your workers to upgrade your weapon? [y/n] ", 27);
}

/**
 * @brief Static callback for showing quit message
 * 
 * Called when 'Q' key is pressed to display quit confirmation.
 */
void ShopCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

/**
 * @brief Display quit message and restore shop interface
 * 
 * Shows quit confirmation message and restores the shop interface
 * with appropriate prompts and instructions.
 */
void ShopCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // Restore previous interface
    UI::ShowInterface("ui/Counters/Shop/shop2.txt");
    UI::DisplayCenterText("Here you can upgrade your weapon to increase its power and fire count.", 24);
    UI::DisplayCenterText("The better your weapon, the longer you can survive!", 25);
    UI::DisplayCenterText("You can only upgrade one level at a time.", 31);
    UI::DisplayCenterText("W: show weapon information | H: return to home | L: show information | Q: quit", 32);
    UI::DisplayCenterText("Do you want to assign one of your workers to upgrade your weapon? [y/n] ", 27);
}

/**
 * @brief Called when exiting the shop system
 * 
 * Cleans up by removing all key callbacks.
 */
void ShopCounter::OnExit() {
    // Clear all callbacks
    clearHKeyCallback();
    clearWKeyCallback();
    clearLKeyCallback();
    clearQKeyCallback();
}
