/**
 * @file UI.h
 * @brief User Interface management class for handling display and interaction
 * 
 * This file defines the UI class, which serves as the main interface manager
 * for the game. It handles all display-related operations including:
 * - Interface rendering and display
 * - Text positioning and formatting
 * - User input prompts
 * - Box-based UI layout management
 * - File-based UI content loading
 * 
 * The UI class provides a consistent interface for displaying game content
 * within a boxed layout, with support for centered text, cursor positioning,
 * and user interaction prompts.
 * 
 * @see Terminal
 * @see Animation
 * @see WeekCycle
 */

#pragma once
#include <string>

class UI {
public:
    /**
     * @brief Displays a UI interface from a file
     * 
     * Loads and displays the contents of a UI file, which contains
     * ASCII art or formatted text for game interfaces.
     * @param filename Path to the UI file to display
     * @return void
     */
    static void ShowInterface(const std::string& filename);
    
    /**
     * @brief Displays the day transition screen
     * 
     * Shows a formatted transition screen when moving to a new day,
     * including the day name and current week number.
     * @param dayName Name of the current day
     * @param currentWeek Current week number
     * @return void
     */
    static void ShowDayTransition(std::string dayName, int currentWeek);
    
    /**
     * @brief Moves the cursor to a specific position within the UI box
     * 
     * Positions the cursor at the specified coordinates relative to
     * the UI box boundaries.
     * @param x X coordinate within the box
     * @param y Y coordinate within the box
     * @return void
     */
    static void MoveCursorInBox(int x, int y);

    /**
     * @brief Waits for user to press Enter
     * 
     * Displays a message and waits for the user to press Enter
     * before continuing.
     * @param message Optional message to display while waiting
     * @return void
     */
    static void WaitForEnter(const std::string& message = "Press Enter to continue...");

    /**
     * @brief Structure containing box boundary coordinates
     */
    struct BoxCoordinates {
        int top;    ///< Top boundary of the box
        int bottom; ///< Bottom boundary of the box
        int left;   ///< Left boundary of the box
        int right;  ///< Right boundary of the box
    };

    /**
     * @brief Gets the coordinates of the UI box
     * 
     * Calculates and returns the boundary coordinates of the UI box
     * based on the content.
     * @param content The content to calculate box coordinates for
     * @return BoxCoordinates containing the box boundaries
     */
    static BoxCoordinates GetBoxCoordinates(const std::string& content);

    /**
     * @brief Loads UI content from a file
     * 
     * Reads and returns the contents of a UI file.
     * @param filename Path to the UI file
     * @return std::string containing the file contents
     */
    static std::string LoadUI(const std::string& filename);

    /**
     * @brief Centers the cursor for text display
     * 
     * Moves the cursor to center a text string within the UI box.
     * @param text The text to center
     * @param lineNumber The line number to center the text on
     * @return void
     */
    static void MoveCursorToCenter(const std::string& text, int lineNumber);

    /**
     * @brief Displays centered text in the UI box
     * 
     * Centers and displays text at the specified line number.
     * @param text The text to display
     * @param lineNumber The line number to display the text on
     * @return void
     */
    static void DisplayCenterText(const std::string& text, int lineNumber);

    /// Box dimensions constants
    static constexpr int BOX_WIDTH = 122;  ///< Width of the UI box (120 '═' + 2 border chars)
    static constexpr int BOX_HEIGHT = 40;  ///< Height of the UI box

private:
    /**
     * @brief Displays UI content
     * 
     * Internal function to display UI content string.
     * @param content The content to display
     * @return void
     */
    static void DisplayUI(const std::string& content);

    /**
     * @brief Displays UI content from file
     * 
     * Internal function to display UI content from a file.
     * @param filename Path to the UI file
     * @return void
     */
    static void DisplayUIFromFile(const std::string& filename);
};