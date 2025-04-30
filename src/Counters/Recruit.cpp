#include "Recruit.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../UI/Animation.h"

RecruitCounter::RecruitCounter(Player& player) 
    : CounterBase(player, "Recruit") {}

void RecruitCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    
    // 之后修改为打字机效果
    UI::ShowInterface("ui/Counters/Recruit/recruit1.txt");
    Animation::TypewriterInBox("Hey! Come to hire someone, huh?", 50, 26);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("Is the pay sweet enough?", 50, 27);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("I'll pick some capable ones for you.", 50, 28);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

void RecruitCounter::Process() {
    int maxPossible = calculateMaxRecruits();
    if (maxPossible <= 0) {
        // to be modified
        UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
        UI::DisplayCenterText("No enough crops to recruit new members!", 25);
        UI::DisplayCenterText("Minimum required: " + std::to_string(BASE_COST + COST_PER_MEMBER) + " crops", 27);
        UI::DisplayCenterText("Current crops: " + std::to_string(m_player.getCrop()) + " crops", 28);
        UI::WaitForEnter("Press Enter to return to home...");
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
        UI::ShowInterface("ui/Counters/Recruit/recruit3.txt");
        Animation::TypewriterInBox("Your soldiers are ready for fight!", 50, 25);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
        UI::DisplayCenterText("Now you have " + std::to_string(recruits) + " more members!", 25);
        UI::DisplayCenterText("Consumed crops: " + std::to_string(totalCost), 27);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
}

int RecruitCounter::GetValidInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
        UI::DisplayCenterText("This is the recruiting office. You can use crops to recruit new members.", 24);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 32);
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] ", 26);
        char yn = Terminal::GetInstance().GetYN();
        if (yn != 'y' && yn != 'Y' && yn != 'n' && yn != 'N'){
            UI::DisplayCenterText("Invalid input! Please enter y or n", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
            continue;
        }else if (yn == 'y' || yn == 'Y'){
            while (true) {
                // 这个txt文件的第一个框里有一个y
                UI::DisplayCenterText("Recruit how many members? (0-" + std::to_string(max) + "): ", 28);

                int input = Terminal::GetInstance().GetInteger();

                if (input >= 0 && input <= max) {
                    UI::DisplayCenterText("Successfully recruited " + std::to_string(input) + " members!", 29);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    return input;
                } else {
                    UI::DisplayCenterText("Must be between 0 and " + std::to_string(max) + "!", 29);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    UI::WaitForEnter("Press Enter to try again...");
                }
           }
        }else if (yn == 'n' || yn == 'N'){
            UI::DisplayCenterText("You will not recruit any member.", 28);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return 0;
        }
    }
}

int RecruitCounter::calculateMaxRecruits() const {
    return std::max(0, (m_player.getCrop() - BASE_COST) / COST_PER_MEMBER);
}