#include "Mining.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include "../UI/Animation.h"
#include <iostream>
#include "../Core/Difficulty.h"

MiningCounter::MiningCounter(Player& player) 
    : CounterBase(player, "Mining") {}

void MiningCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    
    // 之后修改为打字机效果
    UI::ShowInterface("mining1.txt");
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
        UI::ShowInterface("mining3.txt");
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("mining4.txt");
    }

    UI::WaitForEnter("Press enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
}

int MiningCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("mining2.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Assign miners (0-" << max << "): " << std::endl;
    
        int input = Terminal::GetInstance().GetInteger();
        Terminal::GetInstance().MoveCursor(18, 37);
    
        if (input >= 0 && input <= max) {
            std::cout << "Successfully assigned " << input << " miners!" << std::endl;
            return input;
        }
        else {
            std::cout << "Invalid input! Must be between 0 and " << max << "!" << std::endl;
        }
    }
}