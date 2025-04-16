#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include "utilities.h"

namespace utils {
    // clear the terminal screen
    void ClearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    // get a single character without requiring Enter
    char GetKey() {
        char ch;
        struct termios old_settings, new_settings;
        
        // Get current terminal settings
        tcgetattr(STDIN_FILENO, &old_settings);
        new_settings = old_settings;
        
        // Disable canonical mode and echo
        new_settings.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
        
        // Read a single character
        ch = getchar();
        
        // Restore original terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
        
        return ch;
    }

    // load the UI from a file
    std::string LoadUI(const std::string& filename) {
        std::ifstream file(filename);
        std::string content;
        
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                content += line + "\n";
            }
            file.close();
        } else {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
        
        return content;
    }
}

