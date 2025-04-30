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
Terminal::KeyCallback Terminal::qKeyCallback = nullptr;
Terminal::KeyCallback Terminal::hKeyCallback = nullptr;
Terminal::KeyCallback Terminal::wKeyCallback = nullptr;
Terminal::KeyCallback Terminal::lKeyCallback = nullptr;
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

void Terminal::SetQKeyCallback(KeyCallback callback) {
    qKeyCallback = callback;
}

void Terminal::SetHKeyCallback(KeyCallback callback) {
    hKeyCallback = callback;
}

void Terminal::ClearHKeyCallback() {
    hKeyCallback = nullptr;
}

void Terminal::SetWKeyCallback(KeyCallback callback) {
    wKeyCallback = callback;
}

void Terminal::ClearWKeyCallback() {
    wKeyCallback = nullptr;
}

void Terminal::SetLKeyCallback(KeyCallback callback) {
    lKeyCallback = callback;
}

void Terminal::ClearLKeyCallback() {
    lKeyCallback = nullptr;
}

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
    }
    return ch;
}

char Terminal::GetYN() {
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
    return 0; // 返回0表示无效输入
}

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

int Terminal::GetInteger() {
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
            std::cout << ch;  // 只显示数字
            result = result * 10 + (ch - '0');
        }
    }
    return result;
}

bool Terminal::CheckInput() {
    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting > 0;
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

