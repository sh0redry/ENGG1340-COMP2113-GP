/**
 * @file Game.cpp
 * @brief Implementation of the main game logic and state management
 */

#include "Game.h"
#include "Difficulty.h"
#include "../Counters/CounterFactory.h"
#include "../UI/Animation.h"
#include "../UI/Terminal.h"
#include "../Utils/Constants.h"
#include "../UI/UI.h"
#include "../Utils/SpecialFunctions.h"
#include <thread>
#include <iostream>

/**
 * @brief Constructor for the Game class
 * Initializes the game state to MAIN_MENU
 */
Game::Game() : m_state(GameState::MAIN_MENU) {}

/**
 * @brief Main game loop that handles different game states
 * Continuously processes the current game state until the game ends
 * @return void
 */
void Game::run() {
    while (true) {
        Terminal::GetInstance().Clear();
        
        switch (m_state) {
            case GameState::MAIN_MENU: 
                processMainMenu(); 
                break;
            case GameState::DIFFICULTY_SELECT: 
                processDifficultySelect(); 
                break;
            case GameState::PLAYING: 
                processDay(); 
                break;
            case GameState::COMBAT: 
                triggerCombat(); 
                break;
            case GameState::GAME_OVER: 
                return;
        }
    }
}

/**
 * @brief Initializes a new game with the selected difficulty
 * Creates a new player with initial resources based on difficulty settings
 * @return void
 */
void Game::initNewGame() {
    const auto& config = Difficulty::GetConfig(m_difficulty);
    m_player = std::make_unique<Player>(
        config.initialPeople,
        config.initialPeople * 10, // Initial food
        config.initialPeople * 10,  // Initial gold
        m_difficulty              // Difficulty parameter
    );
    m_weekCycle = WeekCycle();
    Animation::PlaySequence("anim/Loading", 70);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

/**
 * @brief Processes the main menu state
 * Handles user input for starting a new game or quitting
 * Displays the main menu interface and animations
 * @return void
 */
void Game::processMainMenu() {
    UI::ShowInterface("ui/empty.txt");
    UI::MoveCursorToCenter("Please adjust your terminal size to make the box fit the screen", 18);
    std::cout << "Please adjust your terminal size to make the box fit the screen" << std::endl;
    UI::WaitForEnter("Press Enter to start...");

    Animation::PlaySequence("anim/Title", 40);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    int selectedOption = 0; // 0 for Play Game, 1 for Quit
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        switch (selectedOption) {
            case 0: UI::ShowInterface("ui/Menu/menu_play.txt"); break;
            case 1: UI::ShowInterface("ui/Menu/menu_quit.txt"); break;
        }
        
        char input = Terminal::GetInstance().GetKeyPress();
        
        switch (input) {
            case 'w':
            case 'W':
                selectedOption = 0;
                break;
            case 's':
            case 'S':
                selectedOption = 1;
                break;
            case '\n': // Enter key
                selectionMade = true;
                break;
        }
    }
    
    if (selectedOption == 0) {
        m_state = GameState::DIFFICULTY_SELECT;
    } else {
        m_state = GameState::GAME_OVER;
    }
}

/**
 * @brief Processes the difficulty selection state
 * Allows player to choose between EASY, MEDIUM, and HARD difficulties
 * @return void
 */
void Game::processDifficultySelect() {
    int selectedOption = 0; // 0 for EASY, 1 for MEDIUM, 2 for HARD
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        switch (selectedOption) {
            case 0: UI::ShowInterface("ui/Difficulty/difficulty_easy.txt"); break;
            case 1: UI::ShowInterface("ui/Difficulty/difficulty_medium.txt"); break;
            case 2: UI::ShowInterface("ui/Difficulty/difficulty_hard.txt"); break;
        }
                
        char input = Terminal::GetInstance().GetKeyPress();
        
        switch (input) {
            case 'w':
            case 'W':
                if (selectedOption > 0) selectedOption--;
                break;
            case 's':
            case 'S':
                if (selectedOption < 2) selectedOption++;
                break;
            case '\n': // Enter key
                selectionMade = true;
                break;
        }
    }
    
    switch (selectedOption) {
        case 0: m_difficulty = "EASY"; break;
        case 1: m_difficulty = "MEDIUM"; break;
        case 2: m_difficulty = "HARD"; break;
    }
    
    initNewGame();
    m_state = GameState::PLAYING;
}

/**
 * @brief Processes a single day in the game
 * Handles daily resource consumption, worker allocation, and game progression
 * @return void
 */
void Game::processDay() {
    // Special handling for the first day with story sequence
    if (m_weekCycle.getCurrentDay() == 1) {
        UI::ShowInterface("ui/empty.txt");
        Animation::TypewriterInBox("The year is 2025.", 50, 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("A zombie outbreak has ravaged HKU, and you must lead a group of survivors", 50, 13);
        Animation::TypewriterInBox("to gather resources, fortify defenses, and endure relentless Thursday hordes. ", 50, 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Every day brings critical choices ———— shop for upgrades, farm for food,", 50, 18);
        Animation::TypewriterInBox("mine for gold, recruiting center, or risk expeditions into the wasteland.", 50, 20);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Will you last long enough for rescue... or fall to the undead?", 100, 23);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        UI::WaitForEnter("Press Enter to continue...");
        UI::ShowInterface("ui/empty.txt");
        Animation::TypewriterInBox("Now, let's start the first day.", 50, 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Today is ......", 50, 18);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        UI::ShowInterface("ui/Days/Friday1.txt");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        UI::WaitForEnter("Press Enter to continue...");
    } else {
        UI::ShowDayTransition(m_weekCycle.getDayName(), m_weekCycle.getCurrentWeek());
    }

    // Reset daily workers and consume daily food
    m_player->resetDailyWorkers();
    m_player->consumeDailyFood();
    
    // Main game loop for the day
    while (m_player->getAvailablePeople() > 0) {
        handleCounterAction();
    }
    
    // Advance to next day
    m_weekCycle.advanceDay();
    
    // Check if next day is Thursday (combat day)
    if (m_weekCycle.isThursday()) {
        m_state = GameState::COMBAT;
    }
    
    // Check game end conditions
    if (m_weekCycle.getCurrentDay() > GameConfig::TOTAL_DAYS) {
        showEndScreen(true);
    } else if (m_player->getPeople() <= 0) {
        showEndScreen(false);
    }
}

/**
 * @brief Handles player interaction with different game counters
 * Manages the 2x3 grid layout of game counters and player navigation
 * @return void
 */
void Game::handleCounterAction() {
    // 2x3 grid layout for counters
    // [0] [1] [2]
    // [3] [ ] [5]
    int selectedOption = 0; // 0-3,5 for the five counters
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        switch (selectedOption) {
            case 0: UI::ShowInterface("ui/Home/home_mining.txt");
                    break;
            case 1: UI::ShowInterface("ui/Home/home_farming.txt");
                    break;
            case 2: UI::ShowInterface("ui/Home/home_shop.txt");
                    break;
            case 3: UI::ShowInterface("ui/Home/home_explore.txt");
                    break;
            case 5: UI::ShowInterface("ui/Home/home_recruit.txt");
                    break;
        }
        
        char input = Terminal::GetInstance().GetKeyPress();
        
        switch (input) {
            case 'w':
            case 'W':
                if (selectedOption > 2) selectedOption -= 3; // Move up
                break;
            case 's':
            case 'S':
                if (selectedOption < 3 && selectedOption != 1) selectedOption += 3; // Move down
                break;
            case 'a':
            case 'A':
                if (selectedOption > 0) {
                    if (selectedOption == 5) selectedOption = 3; // Direct path from 5 to 3
                    else if (selectedOption != 3) selectedOption--; // Normal left movement
                }
                break;
            case 'd':
            case 'D':
                if (selectedOption < 2) selectedOption++; // Normal right movement in first row
                else if (selectedOption == 3) selectedOption = 5; // Direct path from 3 to 5
                else if (selectedOption != 5 && selectedOption != 2) selectedOption++; // Normal right movement
                break;
            case 'l':
            case 'L':
                SpecialFunctions::showPlayerInfo(m_weekCycle, *m_player);
                break;
            case 'q':
            case 'Q':
                SpecialFunctions::showQuitMessage();
                break;
            case '\n': // Enter key
                selectionMade = true;
                break;
        }
    }
    
    // Convert selection to counter type
    CounterType type;
    switch (selectedOption) {
        case 0: type = CounterType::MINING; break;
        case 1: type = CounterType::FARMING; break;
        case 2: type = CounterType::SHOP; break;
        case 3: type = CounterType::EXPLORE; break;
        case 5: type = CounterType::RECRUIT; break;
        default: return;
    }
    
    auto counter = CounterFactory::Create(type, *m_player, m_weekCycle);
    if (!counter) return;
    
    try {
        counter->OnEnter();
        counter->Process();
        counter->OnExit();
    } catch (const ReturnToHomeException&) {
        counter->OnExit();
        return;
    }
}

/**
 * @brief Triggers and manages the combat sequence
 * Handles the zombie attack event, displays combat animations,
 * and processes the combat outcome
 * @return void
 */
void Game::triggerCombat() {
    UI::ShowDayTransition(m_weekCycle.getDayName(), m_weekCycle.getCurrentWeek());

    UI::ShowInterface("ui/empty.txt");
    Animation::TypewriterInBox("You are under attack by a horde of zombies! ! !", 50, 13);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("You must defend your home! ! !", 50, 15);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("The horde is approaching ......", 100, 17);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    UI::WaitForEnter("Press Enter to continue...");

    Animation::PlaySequence("anim/Protect", 100);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    Animation::PlaySequence("anim/Fight", 150);
    UI::WaitForEnter("Press Enter to start fight...");

    Combat combat(*m_player, m_weekCycle);
    bool victory = combat.run();
    if (victory) {
        if (m_weekCycle.getCurrentDay() >= GameConfig::TOTAL_DAYS) {
            showEndScreen(true);
        } else {
            m_weekCycle.advanceDay();  // Advance to next day after victory
            m_state = GameState::PLAYING;
        }
    } else {
        m_state = GameState::GAME_OVER;
    }
}

/**
 * @brief Displays the end game screen
 * Shows victory or defeat animations based on game outcome
 * @param victory Boolean indicating whether the player won or lost
 * @return void
 */
void Game::showEndScreen(bool victory) {
    if (victory) {
        UI::ShowInterface("ui/empty.txt");
        Animation::TypewriterInBox("Congratulations! You have survived the zombie apocalypse!", 50, 13);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("The rescue team is coming soon.", 50, 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("You have saved HKU!", 50, 18);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Thank you for your bravery, my hero!", 50, 20);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        UI::WaitForEnter("Press Enter to quit...");
    } else {
        UI::ShowInterface("ui/empty.txt");
        Animation::TypewriterInBox("Sorry, you have no people left.", 50, 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("You have failed to survive the zombie apocalypse.", 50, 18);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        Animation::PlaySequence("anim/Lose1", 80);
        UI::WaitForEnter("Press Enter to quit...");
        Animation::PlaySequence("anim/Lose2", 100);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    m_state = GameState::GAME_OVER;
}