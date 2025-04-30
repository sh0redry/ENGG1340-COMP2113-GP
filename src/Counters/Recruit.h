#pragma once
#include "CounterBase.h"
#include "../Core/WeekCycle.h"

class RecruitCounter : public CounterBase {
private:
    static constexpr int BASE_COST = 0;
    static constexpr int COST_PER_MEMBER = 30;
    
    // 当前RecruitCounter实例的指针
    static RecruitCounter* currentInstance;
    
    // 显示玩家信息相关方法
    void ShowPlayerInfo();
    static void ShowPlayerInfoCallback();

    // 周循环引用
    WeekCycle& m_weekCycle;

    // 输入状态枚举
    enum class InputState {
        WAITING_YN,     // 等待y/n输入
        WAITING_NUMBER  // 等待数字输入
    };
    InputState m_currentState;

    int GetValidInput(int max);
    int calculateMaxRecruits() const;

public:
    explicit RecruitCounter(Player& player, WeekCycle& weekCycle);
    void OnEnter() override;
    void Process() override;
};