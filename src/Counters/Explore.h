#pragma once
#include "CounterBase.h"
#include "../Utils/Random.h"
#include <string>
#include <unordered_map>

class ExploreCounter : public CounterBase {
public:
    explicit ExploreCounter(Player& player);
    void OnEnter() override;
    void Process() override;

private:
    // 探索结果类型
    enum class ExploreResult {
        GOLD_FOUND,
        CROP_FOUND,
        PEOPLE_JOINED,
        PEOPLE_LOST,
        NOTHING_FOUND,
    };


    // 难度配置表
    static const std::unordered_map<int, float> DIFFICULTY_CONFIGS;

    // 资源奖励范围
    static constexpr int MIN_GOLD_REWARD = 10;
    static constexpr int MAX_GOLD_REWARD = 30;
    static constexpr int MIN_CROP_REWARD = 10;
    static constexpr int MAX_CROP_REWARD = 30;
    static constexpr int MIN_PEOPLE_REWARD = 1;
    static constexpr int MAX_PEOPLE_REWARD = 3;

    // 核心探索逻辑
    ExploreResult executeExplore(int peopleSent);
    
    // 结果处理
    void applyResult(ExploreResult result, int peopleSent, int& value);
    std::vector<std::string> showResultMessage(ExploreResult result, int value);
    
    // UI相关
    int getValidPeopleInput(int max);
};
