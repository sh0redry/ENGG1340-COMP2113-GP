#include "Explore.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include "../Random.h"
#include "../Core/Difficulty.h"
#include <iostream>
#include <string>
#include <vector>

ExploreCounter::ExploreCounter(Player& player) 
    : CounterBase(player, "Explore") {}

void ExploreCounter::OnEnter() {
    // 设置h键回调
    setupHKeyCallback();
    
    // 之后修改为打字机效果
    UI::ShowInterface("explore1.txt");
    UI::WaitForEnter();
}

void ExploreCounter::Process() {
    int peopleSent = getValidPeopleInput(m_player.getAvailablePeople());
    
    if (peopleSent > 0) {
        ExploreResult result = executeExplore(peopleSent);
        int value = 0;
        applyResult(result, peopleSent, value);

        UI::WaitForEnter();
        // 之后修改为打字机效果
        UI::ShowInterface("explore3.txt");
        UI::WaitForEnter();

        // 之后修改为打字机效果
        std::string message = showResultMessage(result, value);
        UI::ShowInterface("explore4.txt");
    }

    UI::WaitForEnter("Press enter to return to home...");
    
    // 清除h键回调
    clearHKeyCallback();
}

// --- 私有方法实现 ---
ExploreCounter::ExploreResult ExploreCounter::executeExplore(int peopleSent) {
    // Get difficulty configuration
    const float lossProbability = DIFFICULTY_CONFIGS.at(m_player.getDifficulty());

    // Check if people are lost
    if (Random::Chance(lossProbability)) {
        return ExploreResult::PEOPLE_LOST;
    }

    // If not lost, evenly distribute remaining probability among three outcomes
    float remainingProbability = (1.0f - lossProbability) / 3.0f;
    float randomValue = Random::RangeReal(0.0f, 1.0f);

    if (randomValue < remainingProbability) {
        return ExploreResult::GOLD_FOUND;
    } else if (randomValue < 2 * remainingProbability) {
        return ExploreResult::CROP_FOUND;
    } else {
        return ExploreResult::PEOPLE_JOINED;
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
            value = Random::Range(MIN_PEOPLE_REWARD, MAX_PEOPLE_REWARD) * peopleSent;
            m_player.addPeople(value);
            break;
            
        case ExploreResult::PEOPLE_LOST:
            value = peopleSent; // 损失全部探索成员
            m_player.addPeople(-value);
            break;
            
        default: break;
    }
}

std::string ExploreCounter::showResultMessage(ExploreResult result, int value) {
    std::string message;
    
    switch (result) {
        case ExploreResult::GOLD_FOUND:
            message = "Exploration Successful! Gold Found: " + std::to_string(value);
            break;
        case ExploreResult::CROP_FOUND:
            message = "Exploration Successful! Food Found: " + std::to_string(value);
            break;
        case ExploreResult::PEOPLE_JOINED:
            message = "Survivors Found! New Members: " + std::to_string(value);
            break;
        case ExploreResult::PEOPLE_LOST:
            message = "Zombies Encountered! Members Lost: " + std::to_string(value);
            break;
        default:
            break;
    }
    return message;
}

int ExploreCounter::getValidPeopleInput(int max) {
    while (true) {
        UI::ShowInterface("explore2.txt");
        Terminal::GetInstance().MoveCursor(18, 33);
        std::cout << "Assign people to explore (0-" << max << "): " << std::endl;
    
        int input = Terminal::GetInstance().GetInteger();
        Terminal::GetInstance().MoveCursor(18, 37);
    
        if (input >= 0 && input <= max) {
            std::cout << "Successfully assigned " << input << " people to explore!" << std::endl;
            return input;
        }
        else {
            std::cout << "Invalid input! Must be between 0 and " << max << "!" << std::endl;
        }
    }
}
