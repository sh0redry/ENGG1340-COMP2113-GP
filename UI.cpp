#include "UI.h"
#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

UI::UI() {
    // Constructor implementation if needed
}

UI::TerminalSize UI::getTerminalSize() const {
    TerminalSize size;
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        size.width = w.ws_col;
        size.height = w.ws_row;
    #endif
    return size;
}

size_t UI::getVisualWidth(const std::string& str) const {
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

void UI::centerAndDisplay(const std::string& content) const {
    TerminalSize terminal = getTerminalSize();
    std::vector<std::string> lines;
    size_t boxWidth = 0;
    size_t pos;
    std::string temp = content;
    
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
    
    int hPadding = (terminal.width - boxWidth) / 2;
    int vPadding = (terminal.height - lines.size()) / 2;
    
    if (hPadding < 0) hPadding = 0;
    if (vPadding < 0) vPadding = 0;
    
    displayLines(lines, hPadding, vPadding);
}

void UI::displayLines(const std::vector<std::string>& lines, int hPadding, int vPadding) const {
    // Add vertical padding before the box
    for (int i = 0; i < vPadding; i++) {
        std::cout << std::endl;
    }
    
    // Display centered lines
    for (const auto& line : lines) {
        std::cout << std::string(hPadding, ' ') << line << std::endl;
    }
    
    // Add vertical padding after the box
    for (int i = 0; i < vPadding - 1; i++) {
        std::cout << std::endl;
    }
}

void UI::showMainMenu() const {
    utils::ClearScreen();
    std::string menu = 
        "==================================\n"
        "        Zombie Survival Game      \n"
        "==================================\n"
        "          1. Start Game           \n"
        "          2. Exit                 \n"
        "==================================\n";
    centerAndDisplay(menu);
}

void UI::showDifficultyMenu() const {
    utils::ClearScreen();
    std::string menu = 
        "==================================\n"
        "        Select Difficulty         \n"
        "==================================\n"
        "          1. Easy                 \n"
        "          2. Medium               \n"
        "          3. Hard                 \n"
        "==================================\n";
    centerAndDisplay(menu);
}

void UI::showHomeScreen() const {
    utils::ClearScreen();
    std::string homeUI = utils::LoadUI("assets/home.txt");
    centerAndDisplay(homeUI);
}

void UI::showCombatScreen() const {
    // TODO: Implement combat screen display
    utils::ClearScreen();
    std::string combatUI = "Combat Screen - To be implemented";
    centerAndDisplay(combatUI);
}

void UI::showGameOver() const {
    utils::ClearScreen();
    std::string gameOver = 
        "==================================\n"
        "           Game Over              \n"
        "==================================\n"
        "    Press any key to continue     \n"
        "==================================\n";
    centerAndDisplay(gameOver);
}

void UI::showVictory() const {
    utils::ClearScreen();
    std::string victory = 
        "==================================\n"
        "          Congratulations!        \n"
        "==================================\n"
        "    You have survived 10 weeks!   \n"
        "    Press any key to continue     \n"
        "==================================\n";
    centerAndDisplay(victory);
}

char UI::getKey() const {
    return utils::GetKey();
}

int UI::getNumberInput() const {
    int input;
    std::cin >> input;
    return input;
} 