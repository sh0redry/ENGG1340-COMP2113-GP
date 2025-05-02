#pragma once
#include <string>
#include <vector>

/**
 * @class WeekCycle
 * @brief Manages the game's weekly cycle and day tracking
 * 
 * The WeekCycle class handles:
 * - Day progression (Friday to Thursday, skipping weekends)
 * - Week counting
 * - Day name retrieval
 * - Special day detection (e.g., Thursday for battles)
 */
class WeekCycle {
private:
    int m_day;          // Current day number (1-25)
    int m_weekdayIndex; // 0=Friday, 1=Monday, 2=Tuesday, 3=Wednesday, 4=Thursday
    
    static const std::vector<std::string> WEEKDAY_NAMES;

public:
    /**
     * @brief Constructs a new WeekCycle object
     * @note Initializes to day 1 (Friday)
     */
    WeekCycle();
    
    /**
     * @brief Advances the game to the next day
     * @note Automatically skips weekends (Saturday and Sunday)
     */
    void advanceDay();
    
    /**
     * @brief Gets the name of the current day
     * @return String representing the current day (Friday, Monday, Tuesday, Wednesday, or Thursday)
     */
    std::string getDayName() const;
    
    /**
     * @brief Checks if the current day is Thursday
     * @return true if current day is Thursday, false otherwise
     * @note Thursday is typically used for battle events
     */
    bool isThursday() const;
    
    /**
     * @brief Gets the total number of days in the game
     * @return Total number of days (from GameConfig::TOTAL_DAYS)
     */
    int getTotalDays() const;
    
    /**
     * @brief Gets the current day number
     * @return Current day number (1-25)
     */
    int getCurrentDay() const;

    /**
     * @brief Gets the current week number
     * @return Current week number (1-5)
     * @note Each week consists of 5 working days (Friday to Thursday)
     */
    int getCurrentWeek() const;
};
