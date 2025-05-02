/**
 * @file WeekCycle.cpp
 * @brief Implementation of the game's time cycle system
 * @details Manages the game's day and week progression, including
 *          weekday tracking, weekend skipping, and special day events.
 */
#include "WeekCycle.h"
#include "../Utils/Constants.h"

/**
 * @brief Names of weekdays in the game
 * @note The order is: Friday, Monday, Tuesday, Wednesday, Thursday
 * @note Saturday and Sunday are not included as they are skipped
 */
const std::vector<std::string> WeekCycle::WEEKDAY_NAMES = {
    "Friday", "Monday", "Tuesday", "Wednesday", "Thursday"
};

/**
 * @brief Constructs a new WeekCycle object
 * @note Initializes to day 1 (Friday)
 */
WeekCycle::WeekCycle() : m_day(1), m_weekdayIndex(0) {} // First day is Friday

/**
 * @brief Advances the game to the next day
 * @note Automatically skips weekends (Saturday and Sunday)
 * @note When reaching Thursday, next day will be Friday
 */
void WeekCycle::advanceDay() {
    m_day++;
    
    // Weekend skipping logic
    if (m_weekdayIndex == 4) { // Current is Thursday
        m_weekdayIndex = 0;    // Next day jumps to Friday
    } else {
        m_weekdayIndex++;      // Otherwise normal progression
    }
    
    // Special handling for weekend skipping
    if (m_weekdayIndex >= 5) {
        m_weekdayIndex = 0;
    }
}

/**
 * @brief Gets the name of the current day
 * @return String representing the current day (Friday, Monday, Tuesday, Wednesday, or Thursday)
 */
std::string WeekCycle::getDayName() const {
    return WEEKDAY_NAMES.at(m_weekdayIndex);
}

/**
 * @brief Checks if the current day is Thursday
 * @return true if current day is Thursday, false otherwise
 * @note Thursday is typically used for battle events
 */
bool WeekCycle::isThursday() const {
    return m_weekdayIndex == 4;
}

/**
 * @brief Gets the total number of days in the game
 * @return Total number of days (from GameConfig::TOTAL_DAYS)
 */
int WeekCycle::getTotalDays() const {
    return GameConfig::TOTAL_DAYS;
}

/**
 * @brief Gets the current day number
 * @return Current day number (1-25)
 */
int WeekCycle::getCurrentDay() const {
    return m_day;
}

/**
 * @brief Gets the current week number
 * @return Current week number (1-5)
 * @note Each week consists of 5 working days (Friday to Thursday)
 */
int WeekCycle::getCurrentWeek() const {
    // Each week has 5 working days (Friday to Thursday)
    return (m_day - 1) / 5 + 1;
}
