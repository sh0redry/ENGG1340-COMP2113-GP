#include "Terminal.h"
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

// 静态成员定义
std::unique_ptr<Terminal> Terminal::instance = nullptr;
std::mutex Terminal::mutex;

// Terminal 类实现
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

Terminal::Terminal() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

Terminal::~Terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void Terminal::Clear() {
    std::cout << "\033[2J\033[1;1H";
}

void Terminal::MoveCursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}

void Terminal::HideCursor() {
    std::cout << "\033[?25l";
}

void Terminal::ShowCursor() {
    std::cout << "\033[?25h";
}

int Terminal::GetKeyPress() {
    return getchar();
}

std::string Terminal::GetLine(int maxLength) {
    std::string input;
    std::getline(std::cin, input);
    if (input.length() > maxLength) {
        input = input.substr(0, maxLength);
    }
    return input;
}

void Terminal::SetColor(Color foreground, Color background) {
    std::cout << "\033[" << static_cast<int>(foreground) << ";" 
              << static_cast<int>(background) + 10 << "m";
}

void Terminal::ResetColor() {
    std::cout << defaultColor;
}

Terminal::TerminalSize Terminal::GetTerminalSize() {
    TerminalSize size;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size.width = w.ws_col;
    size.height = w.ws_row;
    return size;
}

// size_t Terminal::GetVisualWidth(const std::string& str) {
//     size_t width = 0;
//     for (size_t i = 0; i < str.length();) {
//         if ((str[i] & 0xC0) != 0x80) {  // Start of a new character
//             if ((str[i] & 0xF0) == 0xF0) {  // 4-byte UTF-8
//                 i += 4;
//                 width += 2;
//             } else if ((str[i] & 0xE0) == 0xE0) {  // 3-byte UTF-8
//                 i += 3;
//                 width += 2;
//             } else if ((str[i] & 0xC0) == 0xC0) {  // 2-byte UTF-8
//                 i += 2;
//                 width += 2;
//             } else {  // 1-byte character
//                 i += 1;
//                 width += 1;
//             }
//         } else {
//             i += 1;  // Skip continuation byte
//         }
//     }
//     return width;
// }

// void Terminal::CenterAndDisplay(const std::string& content) {
//     TerminalSize terminal = GetTerminalSize();
//     std::vector<std::string> lines;
//     size_t boxWidth = 0;
//     size_t pos;
//     std::string temp = content;
    
//     while ((pos = temp.find('\n')) != std::string::npos) {
//         std::string line = temp.substr(0, pos);
//         boxWidth = std::max(boxWidth, GetVisualWidth(line));
//         lines.push_back(line);
//         temp.erase(0, pos + 1);
//     }
//     if (!temp.empty()) {
//         boxWidth = std::max(boxWidth, GetVisualWidth(temp));
//         lines.push_back(temp);
//     }
    
//     int hPadding = (terminal.width - boxWidth) / 2;
//     int vPadding = (terminal.height - lines.size()) / 2;
    
//     if (hPadding < 0) hPadding = 0;
//     if (vPadding < 0) vPadding = 0;
    
//     DisplayLines(lines, hPadding, vPadding);
// }

// void Terminal::DisplayLines(const std::vector<std::string>& lines, int hPadding, int vPadding) {
//     // Add vertical padding before the box
//     for (int i = 0; i < vPadding; i++) {
//         std::cout << std::endl;
//     }
    
//     // Display centered lines
//     for (const auto& line : lines) {
//         std::cout << std::string(hPadding, ' ') << line << std::endl;
//     }
    
//     // Add vertical padding after the box
//     for (int i = 0; i < vPadding - 1; i++) {
//         std::cout << std::endl;
//     }
// } 