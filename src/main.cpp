/**
 * @file main.cpp
 * @brief Main entry point for the zombie survival game: CRAZY THURSDAY
 * 
 * This file contains the main function that serves as the entry point for the game.
 * It handles game initialization, main game loop execution, and error handling.
 * The program manages terminal display settings and ensures proper cleanup on exit.
 * 
 * @author Group 9
 * @date 2025-05-02
 * @version 1.0
 * 
 * @see Game
 * @see Terminal
 * @see Random
 */

#include "Core/Game.h"
#include "UI/Terminal.h"
#include "Utils/Random.h"
#include <iostream>
#include <thread>
#include <chrono>

/**
 * @brief Main entry point for the game
 * 
 * Initializes the game environment, runs the main game loop, and handles any exceptions.
 * Manages terminal display settings (cursor visibility) and ensures proper cleanup.
 * 
 * The function performs the following operations in sequence:
 * 1. Hides the terminal cursor for better game display
 * 2. Initializes and runs the main game
 * 3. Handles any exceptions that occur during gameplay
 * 4. Ensures the cursor is restored before program exit
 * 
 * @throws std::exception Any exception thrown during game execution
 * @return int Returns 0 on successful execution, non-zero on failure
 */
int main() {
    // Hide cursor for better game display
    Terminal::GetInstance().HideCursor();

    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        Terminal::GetInstance().Clear();
        Terminal::GetInstance().MoveCursor(10, 10);
        std::cerr << "Game crashed: " << e.what();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // Restore cursor visibility before exit
    Terminal::GetInstance().ShowCursor();

    return 0;
}