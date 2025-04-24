#include "UI.h"
#include "Terminal.h"
#include "Animation.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


std::string UI::LoadUI(const std::string& filename) {
    std::ifstream file("assets/" + filename);
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
    
    // 显示内容
    for (int i = 0; i < vPadding; i++) {
        std::cout << std::endl;
    }
    for (const auto& line : lines) {
        std::cout << std::string(hPadding, ' ') << line << std::endl;
    }
    for (int i = 0; i < vPadding - 1; i++) {
        std::cout << std::endl;
    }
}

void UI::DisplayUIFromFile(const std::string& filename) {
    std::string content = LoadUI(filename);
    DisplayUI(content);
}

void UI::ShowInterface(const std::string& filename) {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile(filename);
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
    // 注意：框的边框占用了第一行和第一列，所以需要+1
    int actualX = hPadding + x + 1;
    int actualY = vPadding + y + 1;
    
    // 移动光标
    terminal.MoveCursor(actualX, actualY);
} 