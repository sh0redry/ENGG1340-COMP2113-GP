/**
 * @file Terminal.h
 * @brief Terminal management class for handling console I/O and display
 * 
 * This file defines the Terminal class, which provides functionality for:
 * - Terminal display control (clear, cursor movement)
 * - User input handling
 * - Terminal state management
 * - Key callback registration
 * - Terminal size management
 * 
 * The Terminal class implements the Singleton pattern to ensure
 * consistent terminal state management across the application.
 * 
 * @see UI
 * @see Animation
 */

#pragma once
#include <string>
#include <vector>
#include <termios.h>
#include <mutex>
#include <memory>

class Terminal {
private:
    static std::unique_ptr<Terminal> instance;  ///< Singleton instance
    static std::mutex mutex;                    ///< Mutex for thread safety

    /// Terminal state variables
    struct termios oldt, newt;                 ///< Terminal settings
    std::string defaultColor = "\033[0m";      ///< Default terminal color
    
    /// Key callback function type
    using KeyCallback = void(*)();
    static KeyCallback qKeyCallback;           ///< Callback for 'q' key
    static KeyCallback hKeyCallback;           ///< Callback for 'h' key
    static KeyCallback wKeyCallback;           ///< Callback for 'w' key
    static KeyCallback lKeyCallback;           ///< Callback for 'l' key
    static KeyCallback pKeyCallback;           ///< Callback for 'p' key

    /**
     * @brief Private constructor for singleton pattern
     */
    Terminal();
    
    /// Deleted copy constructor and assignment operator
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    /// Friend declaration for unique_ptr deleter
    friend struct std::default_delete<Terminal>;

public:
    /**
     * @brief Destructor
     * Restores terminal settings
     */
    ~Terminal();

    /**
     * @brief Gets the singleton instance
     * @return Terminal& Reference to the singleton instance
     */
    static Terminal& GetInstance();
    
    /**
     * @brief Sets callback for 'q' key press
     * @param callback Function to call when 'q' is pressed
     */
    static void SetQKeyCallback(KeyCallback callback);
    
    /**
     * @brief Clears 'q' key callback
     */
    static void ClearQKeyCallback();
    
    /**
     * @brief Sets callback for 'h' key press
     * @param callback Function to call when 'h' is pressed
     */
    static void SetHKeyCallback(KeyCallback callback);
    
    /**
     * @brief Clears 'h' key callback
     */
    static void ClearHKeyCallback();
    
    /**
     * @brief Sets callback for 'w' key press
     * @param callback Function to call when 'w' is pressed
     */
    static void SetWKeyCallback(KeyCallback callback);
    
    /**
     * @brief Clears 'w' key callback
     */
    static void ClearWKeyCallback();
    
    /**
     * @brief Sets callback for 'l' key press
     * @param callback Function to call when 'l' is pressed
     */
    static void SetLKeyCallback(KeyCallback callback);
    
    /**
     * @brief Clears 'l' key callback
     */
    static void ClearLKeyCallback();
    
    /**
     * @brief Sets callback for 'p' key press
     * @param callback Function to call when 'p' is pressed
     */
    static void SetPKeyCallback(KeyCallback callback);
    
    /**
     * @brief Clears 'p' key callback
     */
    static void ClearPKeyCallback();
    
    /**
     * @brief Clears the terminal screen
     */
    void Clear();

    /**
     * @brief Moves cursor to specified position
     * @param x X coordinate
     * @param y Y coordinate
     */
    void MoveCursor(int x, int y);

    /**
     * @brief Hides the terminal cursor
     */
    void HideCursor();

    /**
     * @brief Shows the terminal cursor
     */
    void ShowCursor();
    
    /**
     * @brief Gets a single key press from user
     * @return int The ASCII value of the pressed key
     */
    int GetKeyPress();

    /**
     * @brief Gets a yes/no response from user
     * @return char 'Y' or 'N'
     */
    char GetYN();

    /**
     * @brief Gets a line of input from user
     * @param maxLength Maximum length of input
     * @return std::string The input line
     */
    std::string GetLine(int maxLength = 50);

    /**
     * @brief Gets an integer input from user
     * @return int The input integer
     */
    int GetInteger();

    /**
     * @brief Checks if there is any key press available
     * @return bool True if a key is pressed
     */
    bool CheckInput();
    
    /**
     * @brief Structure containing terminal dimensions
     */
    struct TerminalSize {
        int width;   ///< Terminal width in characters
        int height;  ///< Terminal height in characters
    };

    /**
     * @brief Gets the current terminal size
     * @return TerminalSize containing width and height
     */
    TerminalSize GetTerminalSize();
};