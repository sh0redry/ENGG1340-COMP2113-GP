#include "WeekCycle.h"
#include "Constants.h"

const std::vector<std::string> WeekCycle::WEEKDAY_NAMES = {
    "Friday", "Monday", "Tuesday", "Wednesday", "Thursday"
};

WeekCycle::WeekCycle() : m_day(1), m_weekdayIndex(0) {} // 第一天是周五

void WeekCycle::advanceDay() {
    m_day++;
    
    // 周末跳过逻辑
    if (m_weekdayIndex == 4) { // 当前是周四
        m_weekdayIndex = 0;    // 下一天跳到周五
    } else {
        m_weekdayIndex++;      // 否则正常推进
    }
    
    // 跳过周末的特殊处理
    if (m_weekdayIndex >= 5) {
        m_weekdayIndex = 0;
    }
}

std::string WeekCycle::getDayName() const {
    return WEEKDAY_NAMES.at(m_weekdayIndex);
}

bool WeekCycle::isThursday() const {
    return m_weekdayIndex == 4;
}

int WeekCycle::getTotalDays() const {
    return GameConfig::TOTAL_DAYS;
}

int WeekCycle::getCurrentDay() const {
    return m_day;
}