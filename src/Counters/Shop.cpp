#include "Shop.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../Core/Player.h"
#include <iostream>

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
    UI::ShowInterface("shop1.txt");
    UI::WaitForEnter();
}

void ShopCounter::Process() {
    int currentLevel = m_player.getWeaponLevel();
    int cost = Player::WEAPON_UPGRADE_COST.at(currentLevel);

    if (currentLevel >= 10) {
        UI::ShowInterface("shop2_failed.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Have reached the maximum level! " << std::endl;
        std::cout << "Current level: " << m_player.getWeaponLevel() << std::endl;
        UI::WaitForEnter("Press enter to return to home...");
        return;
    } else if (m_player.getGold() < cost) {
        UI::ShowInterface("shop2_failed.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Not enough gold! " << std::endl;
        std::cout << "Cost: " << cost << " gold" << std::endl;
        Terminal::GetInstance().MoveCursor(18, 34);
        std::cout << "Current gold: " << m_player.getGold() << std::endl;
        UI::WaitForEnter("Press enter to return to home...");
        return;
    }

    if (GetValidInput()) {
        m_player.upgradeWeapon();

        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("shop3.txt");
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("shop4.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Current level: " << m_player.getWeaponLevel() << std::endl;
        Terminal::GetInstance().MoveCursor(18, 34);
        std::cout << "Cost: " << cost << " gold" << std::endl;
    }

    UI::WaitForEnter("Press enter to return to home...");

    // 清除h键回调
    clearHKeyCallback();
    // 清除w键回调
    clearWKeyCallback();
}

int ShopCounter::GetValidInput() {
    while (true) {
        UI::ShowInterface("shop2.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        int yn = Terminal::GetInstance().GetYN();
        if (!yn) {
            Terminal::GetInstance().MoveCursor(18, 30);
            std::cout << "Invalid input! Please enter y or n" << std::endl;
            continue;
        }else if (yn == 'y' || yn == 'Y') {
            std::cout << "Successfully upgraded the weapon!" << std::endl;
            return 1;
        }else if (yn == 'n' || yn == 'N') {
            std::cout << "You will not upgrade the weapon." << std::endl;
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
    Terminal::GetInstance().Clear();
    std::cout << "Weapon Level | Power | Fire Count\n";
    std::cout << "--------------------------------\n";
    for (int level = 1; level <= 10; ++level) {
        int power = 10 + level * 5; // 示例：威力随等级线性增长
        int count = 1 + (level - 1) / 3; // 示例：每3级增加一次发射数量
        std::cout << "     " << level << "      |  " << power << "   |     " << count << "\n";
    }
    std::cout << "\nPress 'w' to return..." << std::endl;
    while (true) {
        int ch = getchar();
        if (ch == 'w' || ch == 'W') {
            break;
        }
    }
    Terminal::GetInstance().Clear();
}
