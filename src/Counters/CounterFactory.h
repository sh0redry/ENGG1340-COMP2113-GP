#pragma once
#include "Farming.h"
#include "Mining.h"
#include "Recruit.h"
#include "Shop.h"
#include "Explore.h"
#include <memory>
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include "CounterBase.h"

enum class CounterType {
    FARMING,
    MINING,
    RECRUIT,
    SHOP,
    EXPLORE
};

class CounterFactory {
public:
    static std::unique_ptr<CounterBase> Create(CounterType type, Player& player, WeekCycle& weekCycle) {
        switch (type) {
            case CounterType::FARMING: return std::make_unique<FarmingCounter>(player, weekCycle);
            case CounterType::MINING: return std::make_unique<MiningCounter>(player, weekCycle);
            case CounterType::RECRUIT: return std::make_unique<RecruitCounter>(player, weekCycle);
            case CounterType::SHOP: return std::make_unique<ShopCounter>(player, weekCycle);
            case CounterType::EXPLORE: return std::make_unique<ExploreCounter>(player, weekCycle);
            default: return nullptr;
        }
    }
};
