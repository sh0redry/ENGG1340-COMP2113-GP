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

/**
 * @brief Enumeration of all available counter types in the game
 * 
 * Defines the different types of counters that can be created:
 * - FARMING: For managing crop production
 * - MINING: For managing gold mining
 * - RECRUIT: For recruiting new members
 * - SHOP: For upgrading weapons
 * - EXPLORE: For exploring new territories
 */
enum class CounterType {
    FARMING,
    MINING,
    RECRUIT,
    SHOP,
    EXPLORE
};

/**
 * @brief Factory class for creating different types of counters
 * 
 * Implements the Factory design pattern to create different types of counters
 * based on the specified CounterType. Each counter is created with references
 * to the player and week cycle systems.
 */
class CounterFactory {
public:
    /**
     * @brief Creates a new counter instance based on the specified type
     * @param type The type of counter to create
     * @param player Reference to the player object
     * @param weekCycle Reference to the week cycle system
     * @return Unique pointer to the created counter, or nullptr if type is invalid
     * 
     * Creates and returns a new counter instance of the specified type.
     * The counter is initialized with references to the player and week cycle systems.
     * Returns nullptr if an invalid counter type is specified.
     */
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
