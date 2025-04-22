#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include "utilities.h"

class UI {
private:
    // Terminal dimensions
    struct TerminalSize {
        int width;
        int height;
    };

    // Helper functions
    TerminalSize getTerminalSize() const;
    size_t getVisualWidth(const std::string& str) const;
    void centerAndDisplay(const std::string& content) const;
    void displayLines(const std::vector<std::string>& lines, int hPadding, int vPadding) const;

public:
    // Constructor
    UI();

    // Main menu screens
    void showMainMenu() const;
    void showDifficultyMenu() const;
    void showHomeScreen() const;
    void showCombatScreen() const;
    void showGameOver() const;
    void showVictory() const;

    // Input handling
    char getKey() const;
    int getNumberInput() const;
};

#endif // UI_H 