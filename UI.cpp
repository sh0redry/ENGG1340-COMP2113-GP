#include "UI.h"
#include "Terminal.h"
#include "Animation.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

UI::UI() {
    // Constructor implementation if needed
}

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

void UI::ShowMainMenu() {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile("main_menu.txt");
}

void UI::ShowDifficultyMenu() {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile("difficulty_menu.txt");
}

void UI::ShowHomeScreen(const Player& player, const WeekCycle& week) {
    Terminal::GetInstance().Clear();
    DrawStatusBar(player);
    
    // 显示主界面
    DisplayUIFromFile("home.txt");
    
    // 更新计数器显示
    Terminal::GetInstance().MoveCursor(5, 5);
    std::cout << "[1] 种田 (" << player.getFarmingWorkers() << "人)";
    Terminal::GetInstance().MoveCursor(5, 6);
    std::cout << "[2] 采矿 (" << player.getMiningWorkers() << "人)";
    Terminal::GetInstance().MoveCursor(5, 7);
    std::cout << "[3] 建造 (" << player.getBuildingWorkers() << "人)";
    Terminal::GetInstance().MoveCursor(5, 8);
    std::cout << "[4] 研究 (" << player.getResearchWorkers() << "人)";
    Terminal::GetInstance().MoveCursor(5, 9);
    std::cout << "[5] 战斗 (" << player.getCombatWorkers() << "人)";
    
    // 日期显示
    Terminal::GetInstance().MoveCursor(50, 3);
    std::cout << "第 " << week.getCurrentDay() << " 天 - " << week.getDayName();
}

void UI::ShowCounterMenu(const std::string& counterName) {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile("counter_menu.txt");
    Terminal::GetInstance().MoveCursor(15, 5);
    std::cout << counterName << " 分配";
}

int UI::GetWorkerAllocationInput(int maxAvailable) {
    Terminal::GetInstance().MoveCursor(15, 7);
    std::cout << "当前工人数: ";
    Terminal::GetInstance().MoveCursor(15, 8);
    std::cout << "请输入要分配的工人数: ";
    
    int input;
    std::cin >> input;
    while (input < 0 || input > maxAvailable) {
        Terminal::GetInstance().MoveCursor(15, 9);
        std::cout << "输入无效，请重新输入 (0-" << maxAvailable << "): ";
        std::cin >> input;
    }
    return input;
}

void UI::ShowCombatScreen(int playerHP, int zombieHP, int weaponLevel) {
    Terminal::GetInstance().Clear();
    DisplayUIFromFile("combat.txt");
    
    // 更新状态显示
    Terminal::GetInstance().MoveCursor(10, 5);
    std::cout << "玩家 HP: " << playerHP;
    Terminal::GetInstance().MoveCursor(10, 6);
    std::cout << "武器等级: " << weaponLevel;
    Terminal::GetInstance().MoveCursor(40, 5);
    std::cout << "僵尸 HP: " << zombieHP;
}

void UI::UpdateCombatScreen(int playerHP, int zombieHP) {
    Terminal::GetInstance().MoveCursor(10, 5);
    std::cout << "玩家 HP: " << playerHP;
    Terminal::GetInstance().MoveCursor(40, 5);
    std::cout << "僵尸 HP: " << zombieHP;
}

void UI::ShowGameOver(bool victory) {
    Terminal::GetInstance().Clear();
    if (victory) {
        DisplayUIFromFile("victory.txt");
    } else {
        DisplayUIFromFile("game_over.txt");
    }
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
              << " | 食物: " << UIUtils::FormatResource(player.getCrop())
              << " | 金币: " << UIUtils::FormatResource(player.getGold())
              << " | 武器: Lv." << player.getWeaponLevel();
}

void UI::DrawCounterIcons() {
    Terminal::GetInstance().MoveCursor(5, 5);
    std::cout << "[1] 种田";
    Terminal::GetInstance().MoveCursor(5, 6);
    std::cout << "[2] 采矿";
    Terminal::GetInstance().MoveCursor(5, 7);
    std::cout << "[3] 建造";
    Terminal::GetInstance().MoveCursor(5, 8);
    std::cout << "[4] 研究";
    Terminal::GetInstance().MoveCursor(5, 9);
    std::cout << "[5] 战斗";
}

namespace UIUtils {
    std::string FormatResource(int value) {
        if (value >= 1000) return std::to_string(value/1000) + "K";
        return std::to_string(value);
    }
    
    void DrawBox(int width, int height) {
        std::cout << "+" << std::string(width-2, '-') << "+\n";
        for (int i = 0; i < height-2; ++i) {
            std::cout << "|" << std::string(width-2, ' ') << "|\n";
        }
        std::cout << "+" << std::string(width-2, '-') << "+";
    }
}

// char UI::getKey() {
//     return Terminal::GetInstance().GetKeyPress();
// }

// int UI::getNumberInput() {
//     int input;
//     std::cin >> input;
//     return input;
// } 