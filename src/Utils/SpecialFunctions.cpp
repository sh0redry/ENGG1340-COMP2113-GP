#include "SpecialFunctions.h"
#include "../UI/UI.h"
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include <iostream>


// 显示武器信息的函数
void SpecialFunctions::showWeaponInfo() {
    // 实现武器信息显示
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Weapon Level | Power | Fire Count", 12);
    UI::DisplayCenterText("--------------------------------", 13);
    for (int level = 1; level <= 10; ++level) {
        int power = 10 + level * 5; // 示例：威力随等级线性增长
        int count = 1 + (level - 1) / 3; // 示例：每3级增加一次发射数量
        UI::DisplayCenterText(std::to_string(level) + "      |  " + std::to_string(power) + "   |     " + std::to_string(count), 13 + level);
    }
    UI::DisplayCenterText("Press w to return...", 26);
    while (true) {
        int ch = getchar();
        if (ch == 'w' || ch == 'W') {
            break;
        }
    }
}

// 显示玩家信息的函数
void SpecialFunctions::showPlayerInfo(const WeekCycle& weekCycle, const Player& player) {
    // 实现玩家信息显示
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Player Information", 10);
    UI::DisplayCenterText("----------------", 11);
    UI::DisplayCenterText("Day: " + std::to_string(weekCycle.getCurrentDay()) + " (" + weekCycle.getDayName() + ")", 13);
    UI::DisplayCenterText("Week: " + std::to_string(weekCycle.getCurrentWeek()), 14);
    UI::DisplayCenterText("Gold: " + std::to_string(player.getGold()), 15);
    UI::DisplayCenterText("Crops: " + std::to_string(player.getCrop()), 16);
    UI::DisplayCenterText("Total People: " + std::to_string(player.getPeople()), 17);
    UI::DisplayCenterText("Available People: " + std::to_string(player.getAvailablePeople()), 18);
    UI::DisplayCenterText("Weapon Level: " + std::to_string(player.getWeaponLevel()), 19);
    UI::DisplayCenterText("Difficulty: " + player.getStringDifficulty(), 20);
    UI::DisplayCenterText("Press l to return...", 25);
    
    while (true) {
        int ch = getchar();
        if (ch == 'l' || ch == 'L') {
            break;
        }
    }
}
 