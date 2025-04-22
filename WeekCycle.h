#pragma once
#include <string>
#include <vector>

class WeekCycle {
private:
    int m_day;          // 当前天数（1-25）
    int m_weekdayIndex; // 0=周五,1=周一,2=周二,3=周三,4=周四
    
    static const std::vector<std::string> WEEKDAY_NAMES;

public:
    WeekCycle();
    
    // 推进到下一天（自动跳过周末）
    void advanceDay();
    
    // 获取当前是星期几
    std::string getDayName() const;
    
    // 是否是星期四（战斗日）
    bool isThursday() const;
    
    // 获取总天数
    int getTotalDays() const;
    
    // 获取当前是第几天
    int getCurrentDay() const;
};
