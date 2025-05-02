#include "Farming.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include "../UI/Animation.h"
#include "../Core/Difficulty.h"
#include "../Utils/SpecialFunctions.h"
#include <iostream>
#include <thread>
#include <chrono>

// 初始化静态成员
FarmingCounter* FarmingCounter::currentInstance = nullptr;

FarmingCounter::FarmingCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Farming"), m_weekCycle(weekCycle) {
    currentInstance = this;
}

void FarmingCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    // 设置l键回调
    setupLKeyCallback(ShowPlayerInfoCallback);
    // 设置q键回调
    setupQKeyCallback(ShowQuitMessageCallback);
    
    UI::ShowInterface("ui/Counters/Farming/farming1.txt");
    Animation::TypewriterInBox("Welcome to my farm, guys.", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("It's time to work hard for your survival.", 50, 27);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

void FarmingCounter::Process() {
    const int yield = Difficulty::GetConfig(m_player.getStringDifficulty()).cropYield;
    int workers = GetValidInput(m_player.getAvailablePeople());

    if (workers > 0) {
        m_player.addCrop(yield * workers);
        m_player.assignWorkers(workers, 0, 0, 0, 0);
        
        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Farming/farming3.txt");
        Animation::TypewriterInBox("Your guys are working hard to harvest more crops!", 50, 31);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Farming/farming2.txt");
        UI::DisplayCenterText("You got " + std::to_string(yield * workers) + " crops!", 25);
        UI::DisplayCenterText("You can use the crops to recruit new members and grow your team!", 27);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
}

void FarmingCounter::OnExit() {
    // 清除所有回调
    clearHKeyCallback();
    clearLKeyCallback();
    clearQKeyCallback();
    // 清除当前实例
    currentInstance = nullptr;
}

void FarmingCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

void FarmingCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // 重新显示之前的界面
    UI::ShowInterface("ui/Counters/Farming/farming2.txt");
    UI::DisplayCenterText("Here is a farm, the crops you get from here", 24);
    UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign farmers (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 27);
}

void FarmingCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

void FarmingCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // 重新显示之前的界面
    UI::ShowInterface("ui/Counters/Farming/farming2.txt");
    UI::DisplayCenterText("Here is a farm, the crops you get from here", 24);
    UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign farmers (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 27);
}

int FarmingCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Farming/farming2.txt");
        UI::DisplayCenterText("Here is a farm, the crops you get from here", 24);
        UI::DisplayCenterText("can be used to recruit new members and grow your team!", 25);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
        UI::DisplayCenterText("Assign farmers (0-" + std::to_string(max) + "): ", 27);
    
        int input = Terminal::GetInstance().GetInteger();
    
        if (input >= 0 && input <= max) {
            UI::DisplayCenterText("Successfully assigned " + std::to_string(input) + " farmers!", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return input;
        }
        else {
            UI::DisplayCenterText("Invalid input! Must be between 0 and " + std::to_string(max) + "!", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
        }
    }
}

