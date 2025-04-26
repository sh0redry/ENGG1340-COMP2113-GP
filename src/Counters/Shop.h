#pragma once
#include "CounterBase.h"

class ShopCounter : public CounterBase {
private:
    // 内部方法
    int GetValidInput();
    
    // 当前ShopCounter实例的指针
    static ShopCounter* currentInstance;
    
    // 显示武器信息相关方法
    void ShowWeaponPowerAndLevelInfo();
    static void ShowWeaponPowerAndLevelInfoCallback();
    
public:
    explicit ShopCounter(Player& player);
    void OnEnter() override;
    void Process() override;
};