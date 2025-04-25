#pragma once
#include "CounterBase.h"

class MiningCounter : public CounterBase {
public:
    explicit MiningCounter(Player& player);
    void OnEnter() override;
    void Process() override;
    int GetValidInput(int max);
};