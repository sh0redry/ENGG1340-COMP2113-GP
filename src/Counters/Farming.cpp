#include "Farming.h"
#include "../UI.h"
#include "../Terminal.h"
#include "../Player.h"
#include "../Animation.h"
#include <iostream>

FarmingCounter::FarmingCounter(Player& player) 
    : CounterBase(player, "Farming") {}

void FarmingCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    
    // 之后修改为打字机效果
    UI::ShowInterface("farming1.txt");
    UI::WaitForEnter();
}

void FarmingCounter::Process() {
    const int yield = 10 + 10 * m_player.getDifficulty();
    int workers = GetValidInput(m_player.getAvailablePeople());

    m_player.addCrop(yield * workers);
    m_player.assignWorkers(workers, 0, 0, 0, 0);
    
    UI::WaitForEnter();
    // 之后修改为打字机效果
    UI::ShowInterface("farming3.txt");
    UI::WaitForEnter();

    // 之后修改为打字机效果
    UI::ShowInterface("farming4.txt");
    UI::WaitForEnter("Press enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
}

int FarmingCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("farming2.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Assign farmers (0-" << max << "): " << std::endl;
    
        int input = Terminal::GetInstance().GetInteger();
        Terminal::GetInstance().MoveCursor(18, 37);
    
        if (input >= 0 && input <= max) {
            std::cout << "Successfully assigned " << input << " farmers!" << std::endl;
            return input;
        }
        else {
            std::cout << "Invalid input! Must be between 0 and " << max << "!" << std::endl;
        }
    }
}

