#include "CombatResult.h"
#include <random>
#include <chrono>

int CombatResult::calculateSurvivors(int initialPeople, bool victory) {
    static std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    
    if (victory) {
        // 胜利：50%概率获得1-3人
        std::uniform_int_distribution<int> dist(0, 1);
        if (dist(rng) == 1) {
            std::uniform_int_distribution<int> gain(1, 3);
            return initialPeople + gain(rng);
        }
        return initialPeople;
    } else {
        // 失败：损失30%-70%人员
        std::uniform_int_distribution<int> loss(30, 70);
        return initialPeople * (100 - loss(rng)) / 100;
    }
}

void CombatResult::applyResult(int& people, bool victory) {
    people = calculateSurvivors(people, victory);
} 