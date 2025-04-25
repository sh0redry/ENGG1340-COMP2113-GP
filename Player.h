#pragma once
#include <map>
#include <string>
#include "Constants.h"

class Player {
private:
    // 基础资源
    int m_people;           // 总人数
    int m_availablePeople;  // 可分配人数
    int m_crop;             // 食物资源
    int m_gold;             // 金币资源
    int m_weaponLevel;      // 武器等级（1-10）
    std::string m_difficulty; // 游戏难度

    // 人员分配
    int m_farmingWorkers = 0;
    int m_miningWorkers = 0;
    int m_recruitingWorkers = 0;
    int m_shoppingWorkers = 0;
    int m_exploringWorkers = 0;

    // 常量配置
    static const std::map<int, int> WEAPON_UPGRADE_COST;

public:
    explicit Player(int initialPeople, int initialCrop, int initialGold, const std::string& difficulty = "MEDIUM");

    // 资源获取
    void addCrop(int amount);
    void addGold(int amount);
    void addPeople(int amount);

    // 人员分配
    void assignWorkers(int farming, int mining, int recruiting, int shopping, int exploring);
    void resetDailyWorkers();

    // 商店操作
    bool canUpgradeWeapon() const;
    void upgradeWeapon();

    // 每日消耗
    void consumeDailyFood();

    // 获取状态
    int getDifficulty() const;
    int getPeople() const;
    int getAvailablePeople() const;
    int getCrop() const;
    int getGold() const;
    int getWeaponLevel() const;
    int getTotalHP() const;  // 人数*100

    // 获取各岗位人数
    int getFarmingWorkers() const;
    int getMiningWorkers() const;
    int getRecruitingWorkers() const;
    int getShoppingWorkers() const;
    int getExploringWorkers() const;
};