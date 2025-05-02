#include "SpecialFunctions.h"
#include "../UI/UI.h"
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include "../Combat/Weapon.h"
#include "../UI/Terminal.h"
#include <iostream>
#include <thread>
#include <chrono>


// 显示武器信息的函数
void SpecialFunctions::showWeaponInfo() {
    // 实现武器信息显示
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Weapon Level | Power | Fire Count", 12);
    UI::DisplayCenterText("--------------------------------", 13);
    for (int level = 1; level <= 10; ++level) {
        Weapon weapon(level);
        int power = weapon.getDamage();
        int count = weapon.getMultiple();
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

void SpecialFunctions::showQuitMessage() {
    // 实现退出信息显示
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Are you sure you want to quit?", 15);
    UI::DisplayCenterText("----------------", 16);
    UI::DisplayCenterText("Press q to confirm quit", 19);
    UI::DisplayCenterText("Press any other key to return", 21);
    
    while (true) {
        int ch = getchar();
        if (ch == 'q' || ch == 'Q') {
            UI::ShowInterface("ui/empty.txt");
            UI::DisplayCenterText("Thank you for playing!", 15);
            UI::DisplayCenterText("See you next time!", 17);
            std::cout << std::flush;  // 确保文本被立即显示

            // 增加显示时间到3秒
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            
            // 先显示光标，再清除屏幕
            Terminal::GetInstance().ShowCursor();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            Terminal::GetInstance().Clear();
            exit(0);  // 直接退出游戏
        } else {
            break;
        }
    }
}

bool SpecialFunctions::showPauseScreen() {
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Game Paused", 16);
    UI::DisplayCenterText("----------------", 17);
    UI::DisplayCenterText("Press p to continue", 20);
    
    while (true) {
        int ch = getchar();
        if (ch == 'p' || ch == 'P') {
            return true;  // 返回true表示继续游戏
        }
    }
}