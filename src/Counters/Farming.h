#pragma once
#include "CounterBase.h"
#include "../Core/WeekCycle.h"

class FarmingCounter : public CounterBase {
private:
    // 内部方法
    int GetValidInput(int max);
    
    // 当前FarmingCounter实例的指针
    static FarmingCounter* currentInstance;
    
    // 显示玩家信息相关方法
    void ShowPlayerInfo();
    static void ShowPlayerInfoCallback();

    // 显示退出信息相关方法
    void ShowQuitMessage();
    static void ShowQuitMessageCallback();

    // 周循环引用
    WeekCycle& m_weekCycle;

public:
    explicit FarmingCounter(Player& player, WeekCycle& weekCycle);
    void OnEnter() override;
    void Process() override;
    void OnExit() override;
};