#pragma once
#include "CounterBase.h"

class RecruitCounter : public CounterBase {
public:
    explicit RecruitCounter(Player& player);
    void OnEnter() override;
    void Process() override;
    
private:
    static constexpr int BASE_COST = 100;
    static constexpr int COST_PER_MEMBER = 100;
    
    int GetValidInput(int max);
    int calculateMaxRecruits() const;
};