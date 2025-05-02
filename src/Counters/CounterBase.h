/**
 * @file CounterBase.h
 * @brief Base class for all counter types in the game
 * 
 * This file defines the CounterBase class, which serves as the foundation for all
 * counter types in the game (Farming, Mining, Recruit, Shop, and Explore).
 * It provides common functionality and interface that all counters must implement.
 * 
 * The CounterBase class defines:
 * - Basic counter properties (name, player reference)
 * - Common interface methods (OnEnter, Process, OnExit)
 * - Key callback management
 * - Player information display
 * 
 * @see FarmingCounter
 * @see MiningCounter
 * @see RecruitCounter
 * @see ShopCounter
 * @see ExploreCounter
 */

#pragma once
#include "../Core/Player.h"
#include <string>
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include <stdexcept>

/**
 * @brief Exception class used to interrupt Counter execution and return to home menu
 */
class ReturnToHomeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Return to home menu";
    }
};

/**
 * @brief Base class for all game counters/systems
 * 
 * This abstract class provides the foundation for different game systems
 * like shops, exploration, farming, etc. It handles player interaction
 * and provides common functionality for all derived counters.
 */
class CounterBase {
protected:
    Player& m_player;       // Reference to the player object
    std::string m_name;     // Name of the counter

    /**
     * @brief Static function to trigger return to home menu
     * @throws ReturnToHomeException when called
     */
    static void returnToHome() {
        throw ReturnToHomeException();
    }

public:
    /**
     * @brief Constructor for CounterBase
     * @param player Reference to the player object
     * @param name Name of the counter
     */
    CounterBase(Player& player, const std::string& name)
        : m_player(player), m_name(name) {}
    
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~CounterBase() = default;
    
    /**
     * @brief Pure virtual function called when entering the counter
     * Must be implemented by derived classes
     */
    virtual void OnEnter() = 0;

    /**
     * @brief Pure virtual function for main counter logic
     * Must be implemented by derived classes
     */
    virtual void Process() = 0;
    
    /**
     * @brief Virtual function called when exiting the counter
     * Can be overridden by derived classes
     */
    virtual void OnExit();

    /**
     * @brief Get the name of the counter
     * @return const reference to the counter's name
     */
    const std::string& GetName() const;

    /**
     * @brief Set up the 'H' key callback to return to home menu
     */
    void setupHKeyCallback() {
        Terminal::GetInstance().SetHKeyCallback(returnToHome);
    }
    
    /**
     * @brief Clear the 'H' key callback
     */
    void clearHKeyCallback() {
        Terminal::GetInstance().ClearHKeyCallback();
    }

    /**
     * @brief Set up the 'W' key callback
     * @param callback Function pointer to the callback function
     */
    void setupWKeyCallback(void (*callback)()) {
        Terminal::GetInstance().SetWKeyCallback(callback);
    }

    /**
     * @brief Clear the 'W' key callback
     */
    void clearWKeyCallback() {
        Terminal::GetInstance().ClearWKeyCallback();
    }

    /**
     * @brief Set up the 'L' key callback
     * @param callback Function pointer to the callback function
     */
    void setupLKeyCallback(void (*callback)()) {
        Terminal::GetInstance().SetLKeyCallback(callback);
    }

    /**
     * @brief Clear the 'L' key callback
     */
    void clearLKeyCallback() {
        Terminal::GetInstance().ClearLKeyCallback();
    }

    /**
     * @brief Set up the 'Q' key callback
     * @param callback Function pointer to the callback function
     */
    void setupQKeyCallback(void (*callback)()) {
        Terminal::GetInstance().SetQKeyCallback(callback);
    }

    /**
     * @brief Clear the 'Q' key callback
     */
    void clearQKeyCallback() {
        Terminal::GetInstance().ClearQKeyCallback();
    }
};