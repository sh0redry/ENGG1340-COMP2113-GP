/**
 * @file Terminal.cpp
 * @brief Implementation of terminal control and input handling
 * @details Manages terminal display, cursor control, and user input
 *          with support for key callbacks and terminal state management.
 */
#include "Terminal.h"
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

/// Static member initialization
std::unique_ptr<Terminal> Terminal::instance = nullptr;
std::mutex Terminal::mutex;
Terminal::KeyCallback Terminal::qKeyCallback = nullptr;
Terminal::KeyCallback Terminal::hKeyCallback = nullptr;
Terminal::KeyCallback Terminal::wKeyCallback = nullptr;
Terminal::KeyCallback Terminal::lKeyCallback = nullptr;
Terminal::KeyCallback Terminal::pKeyCallback = nullptr;

/**
 * @brief Gets the singleton instance
 * 
 * Implements thread-safe singleton pattern using double-checked locking.
 * @return Terminal& Reference to the singleton instance
 */
Terminal& Terminal::GetInstance() {
    if (!instance) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!instance) {
            // 使用 C++11 兼容的方式创建 unique_ptr
            instance.reset(new Terminal());
        }
    }
    return *instance;
}

/**
 * @brief Constructor
 * 
 * Initializes terminal settings for raw input mode.
 */
Terminal::Terminal() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

/**
 * @brief Destructor
 * 
 * Restores original terminal settings.
 */
Terminal::~Terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

/**
 * @brief Clears the terminal screen
 * 
 * Uses ANSI escape sequences to clear screen and move cursor to home position.
 */
void Terminal::Clear() {
    std::cout << "\033[2J\033[1;1H";
}

/**
 * @brief Moves cursor to specified position
 * 
 * Uses ANSI escape sequences to position cursor.
 * @param x X coordinate
 * @param y Y coordinate
 */
void Terminal::MoveCursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}

/**
 * @brief Hides the terminal cursor
 * 
 * Uses ANSI escape sequence to hide cursor.
 */
void Terminal::HideCursor() {
    std::cout << "\033[?25l";
}

/**
 * @brief Shows the terminal cursor
 * 
 * Uses ANSI escape sequence to show cursor.
 */
void Terminal::ShowCursor() {
    std::cout << "\033[?25h";
}

/**
 * @brief Sets callback for 'q' key press
 * @param callback Function to call when 'q' is pressed
 */
void Terminal::SetQKeyCallback(KeyCallback callback) {
    qKeyCallback = callback;
}

/**
 * @brief Clears 'q' key callback
 */
void Terminal::ClearQKeyCallback() {
    qKeyCallback = nullptr;
}

/**
 * @brief Sets callback for 'h' key press
 * @param callback Function to call when 'h' is pressed
 */
void Terminal::SetHKeyCallback(KeyCallback callback) {
    hKeyCallback = callback;
}

/**
 * @brief Clears 'h' key callback
 */
void Terminal::ClearHKeyCallback() {
    hKeyCallback = nullptr;
}

/**
 * @brief Sets callback for 'w' key press
 * @param callback Function to call when 'w' is pressed
 */
void Terminal::SetWKeyCallback(KeyCallback callback) {
    wKeyCallback = callback;
}

/**
 * @brief Clears 'w' key callback
 */
void Terminal::ClearWKeyCallback() {
    wKeyCallback = nullptr;
}

/**
 * @brief Sets callback for 'l' key press
 * @param callback Function to call when 'l' is pressed
 */
void Terminal::SetLKeyCallback(KeyCallback callback) {
    lKeyCallback = callback;
}

/**
 * @brief Clears 'l' key callback
 */
void Terminal::ClearLKeyCallback() {
    lKeyCallback = nullptr;
}

/**
 * @brief Sets callback for 'p' key press
 * @param callback Function to call when 'p' is pressed
 */
void Terminal::SetPKeyCallback(KeyCallback callback) {
    pKeyCallback = callback;
}

/**
 * @brief Clears 'p' key callback
 */
void Terminal::ClearPKeyCallback() {
    pKeyCallback = nullptr;
}

/**
 * @brief Gets a single key press from user
 * 
 * Handles special key callbacks and returns the pressed key.
 * @return int The ASCII value of the pressed key
 */
int Terminal::GetKeyPress() {
    int ch = getchar();
    if (ch == 'q' || ch == 'Q') {
        if (qKeyCallback) {
            qKeyCallback();
        }
    } else if (ch == 'h' || ch == 'H') {
        if (hKeyCallback) {
            hKeyCallback();
        }
    } else if (ch == 'w' || ch == 'W') {
        if (wKeyCallback) {
            wKeyCallback();
        }
    } else if (ch == 'l' || ch == 'L') {
        if (lKeyCallback) {
            lKeyCallback();
        }
    } else if (ch == 'p' || ch == 'P') {
        if (pKeyCallback) {
            pKeyCallback();
        }
    }
    return ch;
}

/**
 * @brief Gets a yes/no response from user
 * 
 * Waits for and validates Y/N input, handling special key callbacks.
 * @return char 'Y' or 'N', or 0 for invalid input
 */
char Terminal::GetYN() {
    ShowCursor();
    char ch;
    while ((ch = getchar()) != '\n') {
        if (ch == 'y' || ch == 'Y' || ch == 'n' || ch == 'N') {
            std::cout << ch << std::endl;
            return ch;
        } else if (ch == 'q' || ch == 'Q') {
            if (qKeyCallback) {
                qKeyCallback();
            }
        } else if (ch == 'h' || ch == 'H') {
            if (hKeyCallback) {
                hKeyCallback();
            }
        } else if (ch == 'w' || ch == 'W') {
            if (wKeyCallback) {
                wKeyCallback();
            }
        } else if (ch == 'l' || ch == 'L') {
            if (lKeyCallback) {
                lKeyCallback();
            }
        }
    }
    HideCursor();
    return 0; // Return 0 for invalid input
}

/**
 * @brief Gets a line of input from user
 * 
 * Reads input up to specified length, handling special key callbacks.
 * @param maxLength Maximum length of input
 * @return std::string The input line
 */
std::string Terminal::GetLine(int maxLength) {
    std::string input;
    char ch;
    
    while ((ch = getchar()) != '\n') {
        if (ch == 'q' || ch == 'Q') {
            if (qKeyCallback) {
                qKeyCallback();
            }
        } else if (ch == 'h' || ch == 'H') {
            if (hKeyCallback) {
                hKeyCallback();
            }
        } else if (ch == 'w' || ch == 'W') {
            if (wKeyCallback) {
                wKeyCallback();
            }
        } else if (ch == 'l' || ch == 'L') {
            if (lKeyCallback) {
                lKeyCallback();
            }
        }
        if (input.length() < static_cast<size_t>(maxLength)) {
            input += ch;
        }
    }
    return input;
}

/**
 * @brief Gets an integer input from user
 * 
 * Reads and validates integer input, handling special key callbacks.
 * @return int The input integer
 */
int Terminal::GetInteger() {
    ShowCursor();
    std::string input;
    int result = 0;
    char ch;
    
    while ((ch = getchar()) != '\n') {
        if (ch == 'q' || ch == 'Q') {
            if (qKeyCallback) {
                qKeyCallback();
            }
        } else if (ch == 'h' || ch == 'H') {
            if (hKeyCallback) {
                hKeyCallback();
            }
        } else if (ch == 'w' || ch == 'W') {
            if (wKeyCallback) {
                wKeyCallback();
            }
        } else if (ch == 'l' || ch == 'L') {
            if (lKeyCallback) {
                lKeyCallback();
            }
        }
        if (ch >= '0' && ch <= '9') {
            input += ch;
            std::cout << ch;  // Only display digits
            result = result * 10 + (ch - '0');
        }
    }
    HideCursor();
    return result;
}

/**
 * @brief Checks if there is any key press available
 * 
 * Uses non-blocking input check to detect key presses.
 * @return bool True if a key is pressed
 */
bool Terminal::CheckInput() {
    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting > 0;
}

/**
 * @brief Gets the current terminal size
 * 
 * Retrieves terminal dimensions using ioctl.
 * @return TerminalSize containing width and height
 */
Terminal::TerminalSize Terminal::GetTerminalSize() {
    TerminalSize size;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size.width = w.ws_col;
    size.height = w.ws_row;
    return size;
}

