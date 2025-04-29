#include "UI.h"
#include "Terminal.h"
#include "Animation.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// 计算字符串的视觉宽度（考虑UTF-8字符）
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

std::string UI::LoadUI(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开UI文件: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void UI::DisplayUI(const std::string& content) {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    std::vector<std::string> lines;
    size_t boxWidth = 0;
    size_t pos;
    std::string temp = content;
    
    // 分割内容为行
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
    
    // 计算水平和垂直填充
    int hPadding = (size.width - boxWidth) / 2;
    int vPadding = (size.height - lines.size()) / 2;
    
    // 确保填充值不为负
    if (hPadding < 0) hPadding = 0;
    if (vPadding < 0) vPadding = 0;
    
    // 清除屏幕并移动光标到起始位置
    terminal.Clear();
    terminal.MoveCursor(1, 1);
    
    // 显示内容
    for (int i = 0; i < vPadding; i++) {
        std::cout << std::endl;
    }
    for (const auto& line : lines) {
        // 使用安全的字符串创建方式
        if (hPadding > 0) {
            std::cout << std::string(static_cast<size_t>(hPadding), ' ');
        }
        std::cout << line << std::endl;
    }
    for (int i = 0; i < vPadding - 1; i++) {
        std::cout << std::endl;
    }
    
    // 确保光标在最后一行
    terminal.MoveCursor(1, size.height);
}

void UI::DisplayUIFromFile(const std::string& filename) {
    std::string content = LoadUI(filename);
    DisplayUI(content);
}

// 输入完整路径
void UI::ShowInterface(const std::string& filename) {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile(filename);
}

void UI::WaitForEnter(const std::string& message) {
    // Calculate position for bottom right corner of the box
    // 从边框内侧开始计算，所以x从1开始
    // 考虑双线边框，所以需要+2
    int x = BOX_WIDTH - message.length() - 3;  // -3 for padding (2 for border, 1 for spacing)
    int y = BOX_HEIGHT - 4;  // -3 for padding (2 for border, 1 for spacing)
    
    MoveCursorInBox(x, y);
    std::cout << message;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UI::ShowDayTransition(int day) {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile("day_transition.txt");
    Terminal::GetInstance().MoveCursor(15, 5);
    std::cout << "第 " << day << " 天开始";
    Animation::ProgressBar(2.0f);
}

void UI::DrawStatusBar(const Player& player) {
    Terminal::GetInstance().MoveCursor(1, 1);
    std::cout << "人数: " << player.getPeople() 
              << " | 食物: " << player.getCrop()
              << " | 金币: " << player.getGold()
              << " | 武器: Lv." << player.getWeaponLevel();
}

UI::BoxCoordinates UI::GetBoxCoordinates(const std::string& content) {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    std::vector<std::string> lines;
    size_t boxWidth = 0;
    size_t pos;
    std::string temp = content;
    
    // 分割内容为行
    while ((pos = temp.find('\n')) != std::string::npos) {
        std::string line = temp.substr(0, pos);
        boxWidth = std::max(boxWidth, line.length());
        lines.push_back(line);
        temp.erase(0, pos + 1);
    }
    if (!temp.empty()) {
        boxWidth = std::max(boxWidth, temp.length());
        lines.push_back(temp);
    }
    
    // 计算水平和垂直填充
    int hPadding = (size.width - boxWidth) / 2;
    int vPadding = (size.height - lines.size()) / 2;
    
    if (hPadding < 0) hPadding = 0;
    if (vPadding < 0) vPadding = 0;
    
    BoxCoordinates coords;
    coords.top = vPadding;
    coords.bottom = vPadding + lines.size() - 1;
    coords.left = hPadding;
    coords.right = hPadding + boxWidth - 1;
    
    return coords;
}

void UI::MoveCursorInBox(int x, int y) {
    auto& terminal = Terminal::GetInstance();
    auto size = terminal.GetTerminalSize();
    
    // 计算框在终端中的位置
    int hPadding = (size.width - BOX_WIDTH) / 2;
    int vPadding = (size.height - BOX_HEIGHT) / 2;
    
    if (hPadding < 0) hPadding = 0;
    if (vPadding < 0) vPadding = 0;
    
    // 计算实际的光标位置
    // 注意：双线边框占用了第一行和第一列，所以需要+1
    // 由于是双线边框，内部内容区域从(2,2)开始
    int actualX = hPadding + x + 2;  // 从2开始而不是1
    int actualY = vPadding + y + 2;  // 从2开始而不是1
    
    // 移动光标
    terminal.MoveCursor(actualX, actualY);
} 