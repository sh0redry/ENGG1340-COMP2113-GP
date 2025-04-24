#pragma once
#include "CounterBase.h"

class FarmingCounter : public CounterBase {
public:
    explicit FarmingCounter(Player& player);
    void OnEnter() override;
    void Process() override;
    int GetValidInput(int max);
};