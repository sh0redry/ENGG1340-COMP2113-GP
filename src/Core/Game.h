#pragma once
#include "WeekCycle.h"
#include "Player.h"
#include "../Combat/Combat.h"
#include "../UI/UI.h"
#include "../Counters/CounterBase.h"
#include <memory>

/**
 * @brief Enumeration representing the different states of the game
 * Controls the flow and transitions between different game phases
 */
enum class GameState {
    MAIN_MENU,      ///< Initial menu state where player can start new game or exit
    DIFFICULTY_SELECT, ///< State for selecting game difficulty (EASY, MEDIUM, HARD)
    PLAYING,        ///< Main gameplay state where player manages resources and makes decisions
    COMBAT,         ///< Combat encounter state for zombie attacks
    GAME_OVER       ///< Final state indicating game completion or failure
};

/**
 * @brief Main game class that manages the game state and flow
 * 
 * This class is responsible for:
 * - Managing the game state machine and transitions
 * - Handling player interactions and input
 * - Controlling the main game loop
 * - Managing combat encounters with zombies
 * - Tracking game progress and resources
 * - Displaying UI elements and animations
 */
class Game {
public:
    /**
     * @brief Constructor for the Game class
     * 
     * Initializes the game with default values and sets initial state to MAIN_MENU
     */
    Game();

    /**
     * @brief Main game loop that handles different game states
     * 
     * Continuously processes the current game state until the game ends.
     * Manages state transitions between menu, gameplay, and combat phases.
     * @return void
     */
    void run();

private:
    GameState m_state;           ///< Current state of the game
    std::unique_ptr<Player> m_player;  ///< Pointer to the player object managing resources and stats
    WeekCycle m_weekCycle;       ///< Manages the weekly cycle and day progression
    std::string m_difficulty;    ///< Current game difficulty setting (EASY, MEDIUM, HARD)

    /**
     * @brief Initializes a new game with the selected difficulty
     * 
     * Creates a new player with initial resources based on difficulty settings.
     * Sets up initial game state and displays loading animation.
     * @return void
     */
    void initNewGame();

    /**
     * @brief Processes the main menu state
     * 
     * Handles user input for starting a new game or quitting.
     * Displays the main menu interface and animations.
     * Manages menu navigation and selection.
     * @return void
     */
    void processMainMenu();

    /**
     * @brief Processes the difficulty selection state
     * 
     * Allows player to choose between EASY, MEDIUM, and HARD difficulties.
     * Handles difficulty selection UI and input.
     * @return void
     */
    void processDifficultySelect();

    /**
     * @brief Processes a single day in the game
     * 
     * Handles daily resource consumption, worker allocation, and game progression.
     * Manages special first-day story sequence.
     * Checks for combat triggers and game end conditions.
     * @return void
     */
    void processDay();

    /**
     * @brief Handles player interaction with different game counters
     * 
     * Manages the 2x3 grid layout of game counters and player navigation.
     * Processes counter actions and updates game state accordingly.
     * @return void
     */
    void handleCounterAction();

    /**
     * @brief Triggers and manages the combat sequence
     * 
     * Handles the zombie attack event, displays combat animations,
     * and processes the combat outcome. Manages victory/defeat conditions.
     * @return void
     */
    void triggerCombat();

    /**
     * @brief Displays the end game screen
     * 
     * Shows victory or defeat animations based on game outcome.
     * Displays appropriate end game messages and handles game termination.
     * @param victory Boolean indicating whether the player won (true) or lost (false)
     * @return void
     */
    void showEndScreen(bool victory);
};