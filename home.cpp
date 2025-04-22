//This is the home file 
/*
运行main.cpp后，出现开始界面，点击开始后，出现选择难度界面，选择难度后，进入home页面
home界面有五个counter：挖金币，挖食物，升级武器，招募队员，探险
升级武器需要金币，招募队员需要食物，现存人员每天消耗食物
探险：随机事件，可能获得金币，获得食物，招募到新队员，或死亡

有五个全局变量，会随着游戏进行不断变化：
天数，金币，食物，攻击力（等级），人数

玩法：开局有初始人数，食物，金币，攻击力，第一天是星期五，每天可以分配人数前往不同的counter，每个星期四会遭遇僵尸攻击,
与僵尸的战斗过程是飞机大战的形式，战斗后会有伤亡，若全部死亡则游戏结束，10个星期后还有人或者即可通关

Farm, Mine,
*/

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "utilities.h"

// Function to get terminal dimensions
struct TerminalSize {
    int width;
    int height;
};

// Function to get the visual width of a string (handling Unicode characters)
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

TerminalSize getTerminalSize() {
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

int main() {
    while (true) {
        // Clear screen
        utils::ClearScreen();
        
        // Get terminal dimensions
        TerminalSize terminal = getTerminalSize();
        
        // Load home UI
        std::string homeUI = utils::LoadUI("assets/home.txt");
        
        // Split UI into lines and get dimensions
        std::vector<std::string> uiLines;
        size_t boxWidth = 0;
        size_t pos;
        std::string temp = homeUI;
        
        while ((pos = temp.find('\n')) != std::string::npos) {
            std::string line = temp.substr(0, pos);
            boxWidth = std::max(boxWidth, getVisualWidth(line));
            uiLines.push_back(line);
            temp.erase(0, pos + 1);
        }
        if (!temp.empty()) {
            boxWidth = std::max(boxWidth, getVisualWidth(temp));
            uiLines.push_back(temp);
        }
        
        // Calculate padding for horizontal and vertical centering
        int hPadding = (terminal.width - boxWidth) / 2;
        int vPadding = (terminal.height - uiLines.size()) / 2;
        
        if (hPadding < 0) hPadding = 0;
        if (vPadding < 0) vPadding = 0;
        
        // Add vertical padding before the box
        for (int i = 0; i < vPadding; i++) {
            std::cout << std::endl;
        }
        
        // Display centered lines
        for (const auto& line : uiLines) {
            std::cout << std::string(hPadding, ' ') << line << std::endl;
        }
        
        // Add vertical padding after the box
        for (int i = 0; i < vPadding - 1; i++) {
            std::cout << std::endl;
        }

        // Get user input
        char choice = utils::GetKey();
        
        // Handle user choice
        switch (choice) {
            case '1':
                //function1();
                break;
            case '2':
                //function2();
                break;
            case '3':
                //function3();
                break;
            case '4':
                //function4();
                break;
            case '5':
                //function5();
                break;
            default:
                // If invalid input, continue the loop
                continue;
        }
    }
    
    return 0;
}
