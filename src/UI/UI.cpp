/**
 * @file UI.cpp
 * @brief Implementation of the UI class functionality
 * @details Handles UI rendering, text positioning, and user interaction
 *          in a terminal-based interface.
 */
#include "UI.h"
#include "Terminal.h"
#include "Animation.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../Core/WeekCycle.h"
#include <thread>
#include <chrono>

/**
 * @brief Calculates the visual width of a string considering UTF-8 characters
 * 
 * Handles multi-byte UTF-8 characters correctly when calculating display width.
 * @param str The input string to measure
 * @return size_t The visual width of the string
 */
size_t getVisualWidth(const std::string& str) {
    size_t width = 0;
    for (size_t i = 0; i < str.length();) {
        if ((str[i] & 0xC0) != 0x80) {  // Start of a new character
            if ((str[i] & 0xF0) == 0xF0) {  // 4-byte UTF-8
                i += 4;
                width += 1;
            } else if ((str[i] & 0xE0) == 0xE0) {  // 3-byte UTF-8
                i += 3;
                width += 1;
            } else if ((str[i] & 0xC0) == 0xC0) {  // 2-byte UTF-8
                i += 2;
                width += 1;
            } else {  // 1-byte character
                i += 1;
                width += 1;
            }
        } else {
            i += 1;  // Skip continuation byte
        }
    }
    return width;
}

/**
 * @brief Loads UI content from a file
 * 
 * Reads the entire contents of a UI file into a string.
 * @param filename Path to the UI file
 * @return std::string The file contents
 * @throw std::runtime_error if file cannot be opened
 */
std::string UI::LoadUI(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open UI file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Displays UI content with proper centering
 * 
 * Centers the content both horizontally and vertically in the terminal,
 * handling UTF-8 characters correctly.
 * @param content The content to display
 */
void UI::DisplayUI(const std::string& content) {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    std::vector<std::string> lines;
    size_t boxWidth = 0;
    size_t pos;
    std::string temp = content;
    
    // Split content into lines
    while ((pos = temp.find('\n')) != std::string::npos) {
        std::string line = temp.substr(0, pos);
        boxWidth = std::max(boxWidth, getVisualWidth(line));
        lines.push_back(line);
        temp.erase(0, pos + 1);
    }
    if (!temp.empty()) {
        boxWidth = std::max(boxWidth, getVisualWidth(temp));
        lines.push_back(temp);
    }
    
    // Calculate horizontal and vertical padding
    int hPadding = (size.width - boxWidth) / 2;
    int vPadding = (size.height - lines.size()) / 2;
    
    // Ensure padding values are not negative
    if (hPadding < 0) hPadding = 0;
    if (vPadding < 0) vPadding = 0;
    
    // Clear screen and move cursor to start position
    terminal.Clear();
    terminal.MoveCursor(1, 1);
    
    // Display content
    for (int i = 0; i < vPadding; i++) {
        std::cout << std::endl;
    }
    for (const auto& line : lines) {
        // Use safe string creation method
        if (hPadding > 0) {
            std::cout << std::string(static_cast<size_t>(hPadding), ' ');
        }
        std::cout << line << std::endl;
    }
    for (int i = 0; i < vPadding - 1; i++) {
        std::cout << std::endl;
    }
    
    // Ensure cursor is on the last line
    terminal.MoveCursor(1, size.height);
}

/**
 * @brief Displays UI content from a file
 * 
 * Loads and displays the contents of a UI file.
 * @param filename Path to the UI file
 */
void UI::DisplayUIFromFile(const std::string& filename) {
    std::string content = LoadUI(filename);
    DisplayUI(content);
}

/**
 * @brief Shows a UI interface from a file
 * 
 * Clears the screen and displays the specified UI file.
 * @param filename Path to the UI file
 */
void UI::ShowInterface(const std::string& filename) {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile(filename);
}

/**
 * @brief Waits for user to press Enter
 * 
 * Displays a message and waits for Enter key press,
 * handling input buffer clearing and validation.
 * @param message The message to display
 */
void UI::WaitForEnter(const std::string& message) {
    // Calculate position for bottom right corner of the box
    // Start from inside the border, so x starts from 1
    // Consider double-line border, so need +2
    int x = BOX_WIDTH - message.length() - 3;  // -3 for padding (2 for border, 1 for spacing)
    int y = BOX_HEIGHT - 5;  // -3 for padding (2 for border, 1 for spacing)
    
    // Display the message
    MoveCursorInBox(x, y);
    std::cout << message;
    
    // Completely clear the input buffer and ignore any previous input
    std::cin.clear();
    while (std::cin.rdbuf()->in_avail()) {
        std::cin.ignore();
    }
    
    // Wait for a new Enter key press
    char c;
    do {
        c = std::cin.get();
        // If we get any character other than Enter, ignore it
        if (c != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (c != '\n');
}

/**
 * @brief Shows the day transition screen
 * 
 * Displays an animated transition between game days,
 * including congratulations message and next day information.
 * @param dayName Name of the current day
 * @param currentWeek Current week number
 */
void UI::ShowDayTransition(std::string dayName, int currentWeek) {
    ShowInterface("ui/empty.txt");
    Animation::TypewriterInBox("Congratulations! You survived one more day!", 50, 13);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("Now, let's start a new day!", 50, 15);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("Today is ......", 50, 17);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    std::string filename = dayName + std::to_string(currentWeek) + ".txt";
    ShowInterface("ui/Days/" + filename);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    WaitForEnter("Press Enter to continue...");
}

/**
 * @brief Moves cursor to position within UI box
 * 
 * Positions the cursor at specified coordinates relative to
 * the UI box boundaries.
 * @param x X coordinate within the box
 * @param y Y coordinate within the box
 */
void UI::MoveCursorInBox(int x, int y) {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    
    // Calculate box position in terminal
    int hPadding = (size.width - BOX_WIDTH) / 2;
    int vPadding = (size.height - BOX_HEIGHT) / 2;
    
    if (hPadding < 0) hPadding = 0;
    if (vPadding < 0) vPadding = 0;
    
    // Calculate actual cursor position
    // Note: Double-line border occupies first row and column, so need +1
    // Due to double-line border, inner content area starts from (2,2)
    int actualX = hPadding + x + 2;  // Start from 2 instead of 1
    int actualY = vPadding + y + 2;  // Start from 2 instead of 1
    
    // Move cursor
    terminal.MoveCursor(actualX, actualY);
}

/**
 * @brief Centers cursor for text display
 * 
 * Moves the cursor to center a text string within the UI box.
 * @param text The text to center
 * @param lineNumber The line number to center the text on
 */
void UI::MoveCursorToCenter(const std::string& text, int lineNumber) {
    // Calculate the width of the box (excluding borders)
    int boxContentWidth = BOX_WIDTH - 4;  // -4 for borders (2 on each side)
    
    // Calculate horizontal padding to center the text
    int hPadding = (boxContentWidth - getVisualWidth(text)) / 2;
    
    // Calculate the vertical position
    // lineNumber is 1-based, and we need to account for the top border
    int y = lineNumber + 1;  // +1 to account for the top border
    
    // Move cursor to the calculated position
    MoveCursorInBox(hPadding, y);
}

/**
 * @brief Displays centered text in UI box
 * 
 * Centers and displays text at the specified line number.
 * @param text The text to display
 * @param lineNumber The line number to display the text on
 */
void UI::DisplayCenterText(const std::string& text, int lineNumber) {
    MoveCursorToCenter(text, lineNumber);
    std::cout << text;
}