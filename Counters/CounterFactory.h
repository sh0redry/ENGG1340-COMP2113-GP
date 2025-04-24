#pragma once
#include "Counters/Farming.h"
#include "Counters/Mining.h"
#include "Counters/Recruit.h"
#include "Counters/Shop.h"
#include "Counters/Explore.h"
#include <memory>
#include "Player.h"
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
    static std::unique_ptr<CounterBase> Create(CounterType type, Player& player) {
        switch (type) {
            case CounterType::FARMING: return std::make_unique<FarmingCounter>(player);
            case CounterType::MINING: return std::make_unique<MiningCounter>(player);
            case CounterType::RECRUIT: return std::make_unique<RecruitCounter>(player);
            case CounterType::SHOP: return std::make_unique<ShopCounter>(player);
            case CounterType::EXPLORE: return std::make_unique<ExploreCounter>(player);
            default: return nullptr;
        }
    }
};
