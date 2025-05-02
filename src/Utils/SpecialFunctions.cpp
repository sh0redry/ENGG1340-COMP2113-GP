/**
 * @file SpecialFunctions.cpp
 * @brief Implementation of special game feature functions
 * @details Handles the implementation of various special game features
 *          including weapon information display, player status display,
 *          quit confirmation, and game pause functionality.
 */
#include "SpecialFunctions.h"
#include "../UI/UI.h"
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include "../Combat/Weapon.h"
#include "../UI/Terminal.h"
#include <iostream>
#include <thread>
#include <chrono>

/**
 * @brief Displays weapon information and statistics
 * 
 * Shows a formatted table of weapon levels, their power, and fire count.
 * Displays information for all weapon levels (1-10) in a centered table format.
 * Waits for 'w' key press to return.
 * @return void
 */
void SpecialFunctions::showWeaponInfo() {
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Weapon Level | Power | Fire Count", 12);
    UI::DisplayCenterText("--------------------------------", 13);
    for (int level = 1; level <= 10; ++level) {
        Weapon weapon(level);
        int power = weapon.getDamage();
        int count = weapon.getMultiple();
        UI::DisplayCenterText(std::to_string(level) + "      |  " + std::to_string(power) + "   |     " + std::to_string(count), 13 + level);
    }
    UI::DisplayCenterText("Press w to return...", 26);
    while (true) {
        int ch = getchar();
        if (ch == 'w' || ch == 'W') {
            break;
        }
    }
}

/**
 * @brief Displays current player status and game information
 * 
 * Shows detailed information about the player's current state in a formatted display.
 * Information includes day, week, resources, population, weapon level, and difficulty.
 * Waits for 'l' key press to return.
 * 
 * @param weekCycle Current week cycle information
 * @param player Current player state
 * @return void
 */
void SpecialFunctions::showPlayerInfo(const WeekCycle& weekCycle, const Player& player) {
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Player Information", 10);
    UI::DisplayCenterText("----------------", 11);
    UI::DisplayCenterText("Day: " + std::to_string(weekCycle.getCurrentDay()) + " (" + weekCycle.getDayName() + ")", 13);
    UI::DisplayCenterText("Week: " + std::to_string(weekCycle.getCurrentWeek()), 14);
    UI::DisplayCenterText("Gold: " + std::to_string(player.getGold()), 15);
    UI::DisplayCenterText("Crops: " + std::to_string(player.getCrop()), 16);
    UI::DisplayCenterText("Total People: " + std::to_string(player.getPeople()), 17);
    UI::DisplayCenterText("Available People: " + std::to_string(player.getAvailablePeople()), 18);
    UI::DisplayCenterText("Weapon Level: " + std::to_string(player.getWeaponLevel()), 19);
    UI::DisplayCenterText("Difficulty: " + player.getStringDifficulty(), 20);
    UI::DisplayCenterText("Press l to return...", 25);
    
    while (true) {
        int ch = getchar();
        if (ch == 'l' || ch == 'L') {
            break;
        }
    }
}

/**
 * @brief Displays quit confirmation screen
 * 
 * Shows a confirmation dialog for quitting the game.
 * Handles user input for quit confirmation.
 * If confirmed, displays a farewell message and exits the game.
 * @return void
 */
void SpecialFunctions::showQuitMessage() {
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Are you sure you want to quit?", 15);
    UI::DisplayCenterText("----------------", 16);
    UI::DisplayCenterText("Press q to confirm quit", 19);
    UI::DisplayCenterText("Press any other key to return", 21);
    
    while (true) {
        int ch = getchar();
        if (ch == 'q' || ch == 'Q') {
            UI::ShowInterface("ui/empty.txt");
            UI::DisplayCenterText("Thank you for playing!", 15);
            UI::DisplayCenterText("See you next time!", 17);
            std::cout << std::flush;  // Ensure text is displayed immediately

            // Increase display time to 3 seconds
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            
            // Show cursor before clearing screen
            Terminal::GetInstance().ShowCursor();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            Terminal::GetInstance().Clear();
            exit(0);  // Exit game directly
        } else {
            break;
        }
    }
}

/**
 * @brief Displays and handles game pause screen
 * 
 * Shows the pause menu and handles user input for resuming the game.
 * Waits for 'p' key press to continue.
 * @return bool True if game should continue, false if game should quit
 */
bool SpecialFunctions::showPauseScreen() {
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Game Paused", 16);
    UI::DisplayCenterText("----------------", 17);
    UI::DisplayCenterText("Press p to continue", 20);
    
    while (true) {
        int ch = getchar();
        if (ch == 'p' || ch == 'P') {
            return true;  // Return true to continue game
        }
    }
}