#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Configure terminal for raw input (no line buffering, no echo)
void setTerminalRaw() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Restore terminal to default settings
void restoreTerminal() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Clear screen using ANSI escape codes
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

// Update menu display with selection arrow at specified position
void updateMenuDisplay(const vector<string>& lines, int selected) {
    clearScreen();
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i >= 15 && i <= 17) {  // Menu option lines (rows 15-17)
            if (i - 15 == selected) {
                string line = lines[i];
                if (line.length() < 47) line.resize(47, ' ');  // Ensure enough space for arrow
                line.replace(45, 2, "->");  // Place arrow at column 45
                cout << line << endl;
            } else {
                string line = lines[i];
                if (line.length() < 47) line.resize(47, ' ');
                line.replace(45, 2, "  ");  // Clear arrow position for non-selected items
                cout << line << endl;
            }
        } else {
            cout << lines[i] << endl;  // Output non-option lines unchanged
        }
    }
}

// Read file content into vector of strings
vector<string> readFile(const string& filename) {
    vector<string> lines;
    ifstream in_file(filename);
    string line;
    while (getline(in_file, line)) {
        lines.push_back(line);
    }
    in_file.close();
    return lines;
}

// Handle menu selection and return chosen difficulty as integer
int handleMenuSelection(const string& filename) {
    setTerminalRaw();
    
    vector<string> lines = readFile(filename);
    int selected = 0;  // Initially select first option (Easy)
    updateMenuDisplay(lines, selected);

    while (true) {
        char ch;
        read(STDIN_FILENO, &ch, 1);
        
        if (ch == 'w' || ch == 'W') {
            selected = (selected - 1 + 3) % 3;  // Move selection up (with wrap-around)
            updateMenuDisplay(lines, selected);
        } else if (ch == 's' || ch == 'S') {
            selected = (selected + 1) % 3;  // Move selection down (with wrap-around)
            updateMenuDisplay(lines, selected);
        } else if (ch == '\n') {
            restoreTerminal();
            switch(selected) {
                case 0: 
                    cout << "You selected: Easy" << endl;
                    return 1;  // Return value for Easy
                case 1: 
                    cout << "You selected: Medium" << endl;
                    return 2;  // Return value for Medium
                case 2: 
                    cout << "You selected: Difficult" << endl;
                    return 3;  // Return value for Difficult
            }
        }
    }
}

// Main program entry point
int main() {
    string filename = "menu.txt";  // Menu definition file
    int difficulty = handleMenuSelection(filename);  // Get user's difficulty choice
    return 0;
}
