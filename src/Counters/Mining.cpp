#include "Mining.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include "../UI/Animation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../Core/Difficulty.h"

MiningCounter::MiningCounter(Player& player) 
    : CounterBase(player, "Mining") {}

void MiningCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    
    UI::ShowInterface("ui/Counters/Mining/mining1.txt");
    Animation::TypewriterInBox("Oh! My dear owner, ", 50, 23);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("I've been waiting for you here for a long time!", 50, 24);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("The gold mine seems to have brightened up with your arrival.", 50, 25);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("I'll lead the way for you right away.", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

void MiningCounter::Process() {
    const int yield = Difficulty::GetConfig(m_player.getStringDifficulty()).goldYield;
    int workers = GetValidInput(m_player.getAvailablePeople());

    if (workers > 0) {
        m_player.addGold(yield * workers);
        m_player.assignWorkers(0, workers, 0, 0, 0);
        
        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Mining/mining3.txt");
        Animation::TypewriterInBox("Your guys are working hard to get more golds!", 50, 23);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Mining/mining2.txt");
        UI::DisplayCenterText("You got " + std::to_string(yield * workers) + " golds!", 24);
        UI::DisplayCenterText("You can use the golds to upgrade your weapons and defend yourself against the zombies!", 26);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    UI::WaitForEnter("Press Enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
}

int MiningCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Mining/mining2.txt");
        UI::DisplayCenterText("Here is a gold mine, and the gold you get from here can be used to upgrade your weapons", 24);
        UI::DisplayCenterText("to better defend yourself against the zombies.", 25);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
        UI::DisplayCenterText("Assign miners (0-" + std::to_string(max) + "): ", 27);
    
        int input = Terminal::GetInstance().GetInteger();
    
        if (input >= 0 && input <= max) {
            UI::DisplayCenterText("Successfully assigned " + std::to_string(input) + " miners!", 28);
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