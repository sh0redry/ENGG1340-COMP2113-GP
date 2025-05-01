#include "Recruit.h"
#include "../UI/UI.h"
#include "../UI/Terminal.h"
#include "../Core/Player.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../UI/Animation.h"
#include "../Utils/SpecialFunctions.h"

// 初始化静态成员
RecruitCounter* RecruitCounter::currentInstance = nullptr;

RecruitCounter::RecruitCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Recruit"), m_weekCycle(weekCycle), m_currentState(InputState::WAITING_YN) {
    currentInstance = this;
}

void RecruitCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    // 设置l键回调
    setupLKeyCallback(ShowPlayerInfoCallback);
    // 设置q键回调
    setupQKeyCallback(ShowQuitMessageCallback);
    
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
    // 清除l键回调
    clearLKeyCallback();
    // 清除q键回调
    clearQKeyCallback();
}

void RecruitCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

void RecruitCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // 根据当前状态重新显示对应的界面
    UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
    UI::DisplayCenterText("This is the recruiting office. You can use crops to recruit new members.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 32);
    
    if (m_currentState == InputState::WAITING_YN) {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] ", 26);
    } else {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] y", 26);
        UI::DisplayCenterText("Recruit how many members? (0-" + std::to_string(calculateMaxRecruits()) + "): ", 28);
    }
}

int RecruitCounter::GetValidInput(int max) {
    while (true) {
        m_currentState = InputState::WAITING_YN;
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
                m_currentState = InputState::WAITING_NUMBER;
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

void RecruitCounter::ShowQuitMessageCallback() {
    if (currentInstance) {
        currentInstance->ShowQuitMessage();
    }
}

void RecruitCounter::ShowQuitMessage() {
    SpecialFunctions::showQuitMessage();
    // 重新显示之前的界面
    UI::ShowInterface("ui/Counters/Recruit/recruit2.txt");
    UI::DisplayCenterText("This is the recruiting office. You can use crops to recruit new members.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 32);
    
    if (m_currentState == InputState::WAITING_YN) {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] ", 26);
    } else {
        UI::DisplayCenterText("Do you want to assign one of your workers to recruit new members? [y/n] y", 26);
        UI::DisplayCenterText("Recruit how many members? (0-" + std::to_string(calculateMaxRecruits()) + "): ", 28);
    }
}