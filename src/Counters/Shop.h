#pragma once
#include "CounterBase.h"
#include "../Core/WeekCycle.h"

class ShopCounter : public CounterBase {
private:
    // 内部方法
    int GetValidInput();
    
    // 当前ShopCounter实例的指针
    static ShopCounter* currentInstance;
    
    // 显示武器信息相关方法
    void ShowWeaponPowerAndLevelInfo();
    static void ShowWeaponPowerAndLevelInfoCallback();

    // 显示玩家信息相关方法
    void ShowPlayerInfo();
    static void ShowPlayerInfoCallback();

    // 周循环引用
    WeekCycle& m_weekCycle;

public:
    explicit ShopCounter(Player& player, WeekCycle& weekCycle);
    void OnEnter() override;
    void Process() override;
};