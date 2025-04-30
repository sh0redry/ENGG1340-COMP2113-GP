#include "Shop.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../Core/Player.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../UI/Animation.h"
// 静态成员初始化
ShopCounter* ShopCounter::currentInstance = nullptr;

ShopCounter::ShopCounter(Player& player) 
    : CounterBase(player, "Shop") {}

void ShopCounter::OnEnter() {
    // 设置当前实例
    currentInstance = this;
    // 设置h键回调
    setupHKeyCallback();
    // 设置w键回调
    setupWKeyCallback(ShowWeaponPowerAndLevelInfoCallback);
    // 之后修改为打字机效果
    UI::ShowInterface("ui/Counters/Shop/shop1.txt");
    Animation::TypewriterInBox("Yo, what's up? Wanna upgrade your weapon?", 50, 24);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("Gotta see if you've got the skills and the dough.", 50, 25);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("That'll cost ya 30kg of the shiny gold per level.", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("If you can't afford, don't waste my smoke time.", 50, 27);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

void ShopCounter::Process() {
    int currentLevel = m_player.getWeaponLevel();
    int cost = Player::WEAPON_UPGRADE_COST.at(currentLevel);

    if (currentLevel >= 10) {
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Have reached the maximum level! ", 24);
        UI::DisplayCenterText("Current level: " + std::to_string(m_player.getWeaponLevel()), 26);
        UI::WaitForEnter("Press Enter to return to home...");
        return;
    } else if (m_player.getGold() < cost) {
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Not enough gold to upgrade your weapon! ", 24);
        UI::DisplayCenterText("Cost: " + std::to_string(cost) + " gold", 26);
        UI::DisplayCenterText("Current gold: " + std::to_string(m_player.getGold()), 27);
        UI::WaitForEnter("Press Enter to return to home...");
        return;
    }

    if (GetValidInput()) {
        m_player.upgradeWeapon();
        m_player.assignWorkers(0, 0, 0, 1, 0);
        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Shop/shop3.txt");
        Animation::TypewriterInBox("Um, what a beautiful and powerful weapon!", 50, 24);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("It can easily destroy a whole zombie horde!", 50, 25);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Current level: " + std::to_string(m_player.getWeaponLevel()), 25);
        UI::DisplayCenterText("Cost: " + std::to_string(cost) + " gold", 27);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    UI::WaitForEnter("Press Enter to return to home...");

    // 清除h键回调
    clearHKeyCallback();
    // 清除w键回调
    clearWKeyCallback();
}

int ShopCounter::GetValidInput() {
    while (true) {
        UI::ShowInterface("ui/Counters/Shop/shop2.txt");
        UI::DisplayCenterText("Here you can upgrade your weapon to increase its power and fire count.", 24);
        UI::DisplayCenterText("The better your weapon, the longer you can survive!", 25);
        UI::DisplayCenterText("You can only upgrade one level at a time.", 31);
        UI::DisplayCenterText("W: show weapon information | H: return to home | L: show information | Q: quit", 32);
        UI::DisplayCenterText("Do you want to assign one of your workers to upgrade your weapon? [y/n] ", 27);
        char yn = Terminal::GetInstance().GetYN();
        if (yn != 'y' && yn != 'Y' && yn != 'n' && yn != 'N'){
            UI::DisplayCenterText("Invalid input! Please enter y or n", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
            continue;
        }else if (yn == 'y' || yn == 'Y') {
            UI::DisplayCenterText("Successfully upgraded the weapon!", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return 1;
        }else if (yn == 'n' || yn == 'N') {
            UI::DisplayCenterText("You will not upgrade the weapon.", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return 0;
        }
    }
}

// 静态回调函数
void ShopCounter::ShowWeaponPowerAndLevelInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowWeaponPowerAndLevelInfo();
    }
}

// 非静态方法：显示武器威力和发射数量与等级关系
void ShopCounter::ShowWeaponPowerAndLevelInfo() {
    UI::ShowInterface("ui/empty.txt");
    UI::DisplayCenterText("Weapon Level | Power | Fire Count", 12);
    UI::DisplayCenterText("--------------------------------", 13);
    for (int level = 1; level <= 10; ++level) {
        int power = 10 + level * 5; // 示例：威力随等级线性增长
        int count = 1 + (level - 1) / 3; // 示例：每3级增加一次发射数量
        UI::DisplayCenterText(std::to_string(level) + "      |  " + std::to_string(power) + "   |     " + std::to_string(count), 13 + level);
    }
    UI::DisplayCenterText("Press w to return...", 25);
    while (true) {
        int ch = getchar();
        if (ch == 'w' || ch == 'W') {
            break;
        }
    }
    // 重新显示之前的界面
    UI::ShowInterface("ui/Counters/Shop/shop2.txt");
    UI::DisplayCenterText("Here you can upgrade your weapon to increase its power and fire count.", 24);
    UI::DisplayCenterText("The better your weapon, the longer you can survive!", 25);
    UI::DisplayCenterText("You can only upgrade one level at a time.", 31);
    UI::DisplayCenterText("W: show weapon information | H: return to home | L: show information | Q: quit", 32);
    UI::DisplayCenterText("Do you want to assign one of your workers to upgrade your weapon? [y/n] ", 27);
}
