#include "Recruit.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include <iostream>

RecruitCounter::RecruitCounter(Player& player) 
    : CounterBase(player, "Recruit") {}

void RecruitCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    
    // 之后修改为打字机效果
    UI::ShowInterface("recruit1.txt");
    UI::WaitForEnter();
}

void RecruitCounter::Process() {
    int maxPossible = calculateMaxRecruits();
    if (maxPossible <= 0) {
        // to be modified
        UI::ShowInterface("recruit2_failed.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "No enough food!" << std::endl;
        Terminal::GetInstance().MoveCursor(18, 34);
        std::cout << "Minimum required: " << std::to_string(BASE_COST + COST_PER_MEMBER) << " food" << std::endl;
        Terminal::GetInstance().MoveCursor(18, 35);
        std::cout << "Current food: " << std::to_string(m_player.getCrop()) << " food" << std::endl;
        UI::WaitForEnter("Press enter to return to home...");
        return;
    }
    
    int recruits = GetValidInput(maxPossible);
    
    if (recruits > 0) {
        int totalCost = BASE_COST + COST_PER_MEMBER * recruits;
        m_player.addCrop(-totalCost);
        m_player.addPeople(recruits);
        m_player.assignWorkers(0, 0, 1, 0, 0);
        
        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("recruit3.txt");
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("recruit4.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Now you have " << recruits << " more members!" << std::endl;
        Terminal::GetInstance().MoveCursor(18, 34);
        std::cout << "Consumed food: " << totalCost << std::endl;
    }
    
    UI::WaitForEnter("Press enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
}

int RecruitCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("recruit2.txt");
        Terminal::GetInstance().MoveCursor(18, 27);
        int yn = Terminal::GetInstance().GetYN();
        if (!yn){
            Terminal::GetInstance().MoveCursor(18, 30);
            std::cout << "Invalid input! Please enter y or n" << std::endl;
            continue;
        }else if (yn == 'y' || yn == 'Y'){
            while (true) {
                UI::ShowInterface("recruit2_yes.txt");
                // 这个txt文件的第一个框里有一个y
                Terminal::GetInstance().MoveCursor(18, 33);
                std::cout << "Recruit how many members? (0-" << max << "): " << std::endl;

                int input = Terminal::GetInstance().GetInteger();
                Terminal::GetInstance().MoveCursor(18, 37);

                if (input >= 0 && input <= max) {
                    std::cout << "Successfully recruited " << input << " members!" << std::endl;
                    return input;
                } else {
                    std::cout << "Must be between 0 and " << max << "!" << std::endl;
                }
           }
        }else if (yn == 'n' || yn == 'N'){
            std::cout << "You will not recruit any member." << std::endl;
            return 0;
        }
    }
}

int RecruitCounter::calculateMaxRecruits() const {
    return std::max(0, (m_player.getCrop() - BASE_COST) / COST_PER_MEMBER);
}