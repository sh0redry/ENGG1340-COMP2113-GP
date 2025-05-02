/**
 * @file SpecialFunctions.h
 * @brief Utility functions for special game features and displays
 * 
 * This file defines the SpecialFunctions class, which provides utility functions
 * for various special game features including:
 * - Weapon information display
 * - Player status display
 * - Quit confirmation
 * - Game pause functionality
 * 
 * These functions handle the display and interaction of special game features
 * that are used across different parts of the game.
 * 
 * @see Player
 * @see WeekCycle
 * @see UI
 */

#pragma once
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include "../UI/UI.h"
#include <iostream>

class SpecialFunctions {
public:
    /**
     * @brief Displays weapon information and statistics
     * 
     * Shows a table of weapon levels, their power, and fire count.
     * Displays information for all weapon levels (1-10).
     * @return void
     */
    static void showWeaponInfo();

    /**
     * @brief Displays current player status and game information
     * 
     * Shows detailed information about the player's current state including:
     * - Current day and week
     * - Resource amounts (gold, crops)
     * - Population statistics
     * - Weapon level
     * - Game difficulty
     * 
     * @param weekCycle Current week cycle information
     * @param player Current player state
     * @return void
     */
    static void showPlayerInfo(const WeekCycle& weekCycle, const Player& player);

    /**
     * @brief Displays quit confirmation screen
     * 
     * Shows a confirmation dialog for quitting the game.
     * Handles user input for quit confirmation.
     * @return void
     */
    static void showQuitMessage();

    /**
     * @brief Displays and handles game pause screen
     * 
     * Shows the pause menu and handles user input for resuming the game.
     * @return bool True if game should continue, false if game should quit
     */
    static bool showPauseScreen();
}; 