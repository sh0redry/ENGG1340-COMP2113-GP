#include "Explore.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../Utils/Random.h"
#include "../Core/Difficulty.h"
#include "../Core/Player.h"
#include "../UI/Animation.h"
#include "../Core/WeekCycle.h"
#include "../Utils/SpecialFunctions.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

// 初始化静态成员
ExploreCounter* ExploreCounter::currentInstance = nullptr;

ExploreCounter::ExploreCounter(Player& player, WeekCycle& weekCycle) 
    : CounterBase(player, "Explore"), m_weekCycle(weekCycle) {
    currentInstance = this;
}

void ExploreCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    // 设置l键回调
    setupLKeyCallback(ShowPlayerInfoCallback);
    
    UI::ShowInterface("ui/Counters/Explore/explore1.txt");
    Animation::TypewriterInBox("Step into this land, and ahead lies the mysterious and uncharted territory,", 50, 16);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("fraught with perils yet hiding endless treasures,", 50, 17);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Animation::TypewriterInBox("where high risks and high rewards go hand in hand ~", 50, 18);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    UI::WaitForEnter();
}

void ExploreCounter::Process() {
    int peopleSent = getValidPeopleInput(m_player.getAvailablePeople());
    
    if (peopleSent > 0) {
        m_player.assignWorkers(0, 0, 0, 0, peopleSent);
        ExploreResult result = executeExplore(peopleSent);
        int value = 0;
        applyResult(result, peopleSent, value);

        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("ui/Counters/Explore/explore1.txt");
        Animation::TypewriterInBox("Your guys are exploring the land, which is full of perils and treasures!", 50, 16);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Will they return with treasures or in pieces?", 50, 17);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Let's pray for them ......", 50, 18);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        UI::WaitForEnter();

        // 之后修改为打字机效果
        std::vector<std::string> messages = showResultMessage(result, value);
        UI::ShowInterface("ui/Counters/Explore/explore2.txt");
        UI::DisplayCenterText(messages[0], 23);
        UI::DisplayCenterText(messages[1], 25);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    UI::WaitForEnter("Press Enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
    // 清除l键回调
    clearLKeyCallback();
}

// --- 私有方法实现 ---
ExploreCounter::ExploreResult ExploreCounter::executeExplore(int peopleSent) {
    // Get difficulty configuration
    const float lossProbability = Difficulty::GetConfig(m_player.getStringDifficulty()).exploreRisk;

    // Check if people are lost
    if (Random::Chance(lossProbability)) {
        return ExploreResult::PEOPLE_LOST;
    }

    // If not lost, distribute probability according to 2:2:2:1 ratio
    float remainingProbability = (1.0f - lossProbability) / 7.0f; // Total parts = 2+2+2+1 = 7
    float randomValue = Random::RangeReal(0.0f, 1.0f);

    if (randomValue < 2 * remainingProbability) {
        return ExploreResult::GOLD_FOUND;
    } else if (randomValue < 4 * remainingProbability) {
        return ExploreResult::CROP_FOUND;
    } else if (randomValue < 6 * remainingProbability) {
        return ExploreResult::PEOPLE_JOINED;
    } else {
        return ExploreResult::NOTHING_FOUND;
    }
}

void ExploreCounter::applyResult(ExploreResult result, int peopleSent, int& value) {
    switch (result) {
        case ExploreResult::GOLD_FOUND:
            value = Random::Range(MIN_GOLD_REWARD, MAX_GOLD_REWARD) * peopleSent;
            m_player.addGold(value);
            break;
            
        case ExploreResult::CROP_FOUND:
            value = Random::Range(MIN_CROP_REWARD, MAX_CROP_REWARD) * peopleSent;
            m_player.addCrop(value);
            break;
            
        case ExploreResult::PEOPLE_JOINED:
            value = Random::Range(MIN_PEOPLE_REWARD, MAX_PEOPLE_REWARD);
            m_player.addPeople(value);
            break;
            
        case ExploreResult::PEOPLE_LOST:
            value = peopleSent; // 损失全部探索成员
            m_player.addPeople(-value);
            break;
            
        case ExploreResult::NOTHING_FOUND:
            value = 0;
            break;
            
        default: break;
    }
}

std::vector<std::string> ExploreCounter::showResultMessage(ExploreResult result, int value) {
    std::vector<std::string> messages;
    
    switch (result) {
        case ExploreResult::GOLD_FOUND:
            messages.push_back("Exploration Successful! Congratulations!");
            messages.push_back("Gold Found: " + std::to_string(value));
            break;
        case ExploreResult::CROP_FOUND:
            messages.push_back("Exploration Successful! Congratulations!");
            messages.push_back("Food Found: " + std::to_string(value));
            break;
        case ExploreResult::PEOPLE_JOINED:
            messages.push_back("Congratulations! Survivors Found!");
            messages.push_back("New Members: " + std::to_string(value));
            break;
        case ExploreResult::PEOPLE_LOST:
            messages.push_back("OHHH NO!!! They sacrificed!");
            messages.push_back("Members Lost: " + std::to_string(value));
            break;
        case ExploreResult::NOTHING_FOUND:
            messages.push_back("Exploration Completed.");
            messages.push_back("Nothing was found.");
            break;
        default:
            break;
    }
    return messages;
}

void ExploreCounter::ShowPlayerInfoCallback() {
    if (currentInstance) {
        currentInstance->ShowPlayerInfo();
    }
}

void ExploreCounter::ShowPlayerInfo() {
    SpecialFunctions::showPlayerInfo(m_weekCycle, m_player);
    // 重新显示之前的界面
    UI::ShowInterface("ui/Counters/Explore/explore2.txt");
    UI::DisplayCenterText("Type in the number of people you want to assign to this land!", 22);
    UI::DisplayCenterText("Think twice before you decide!", 23);
    UI::DisplayCenterText("The rewards are tempting, but you could die for it.", 24);
    UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
    UI::DisplayCenterText("Assign people to explore (0-" + std::to_string(m_player.getAvailablePeople()) + "): ", 26);
}

int ExploreCounter::getValidPeopleInput(int max) {
    while (true) {
        UI::ShowInterface("ui/Counters/Explore/explore2.txt");
        UI::DisplayCenterText("Type in the number of people you want to assign to this land!", 22);
        UI::DisplayCenterText("Think twice before you decide!", 23);
        UI::DisplayCenterText("The rewards are tempting, but you could die for it.", 24);
        UI::DisplayCenterText("Enter: confirm | H: return to home | L: show information | Q: quit", 31);
        UI::DisplayCenterText("Assign people to explore (0-" + std::to_string(max) + "): ", 26);
    
        int input = Terminal::GetInstance().GetInteger();
    
        if (input >= 0 && input <= max) {
            UI::DisplayCenterText("Successfully assigned " + std::to_string(input)  + " people to explore!", 27);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return input;
        }
        else {
            UI::DisplayCenterText("Invalid input! Must be between 0 and " + std::to_string(max) + "!", 27);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            UI::WaitForEnter("Press Enter to try again...");
        }
    }
}
