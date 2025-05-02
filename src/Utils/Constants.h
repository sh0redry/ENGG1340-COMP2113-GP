#pragma once

/**
 * @brief Game configuration constants
 * 
 * This namespace contains fundamental game configuration parameters that
 * define the game's structure and basic mechanics. These constants are
 * used throughout the game to maintain consistency in game rules and
 * balance.
 */
namespace GameConfig {
    /**
     * @brief Game time configuration
     * 
     * Defines the temporal structure of the game:
     * - TOTAL_WEEKS: Total number of weeks in the game
     * - DAYS_PER_WEEK: Number of days in each week
     * - TOTAL_DAYS: Total number of days in the game
     */
    const int TOTAL_WEEKS = 5;
    const int DAYS_PER_WEEK = 5;
    const int TOTAL_DAYS = TOTAL_WEEKS * DAYS_PER_WEEK;
    
    /**
     * @brief Resource and unit configuration
     * 
     * Defines basic resource requirements and unit properties:
     * - FOOD_PER_PERSON: Daily food consumption per person
     * - BASE_HP: Starting health points for units
     * - RECRUIT_COST: Cost to recruit a new person
     */
    const int FOOD_PER_PERSON = 5;
    const int BASE_HP = 100;
    const int RECRUIT_COST = 30;
}
