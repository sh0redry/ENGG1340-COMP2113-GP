#include "Player.h"
#include "Difficulty.h"

const std::map<int, int> Player::WEAPON_UPGRADE_COST = {
    {1, 100}, {2, 150}, {3, 200}, {4, 250}, {5, 300},
    {6, 350}, {7, 400}, {8, 450}, {9, 500}, {10, 600}
};

Player::Player(int initialPeople, int initialCrop, int initialGold, const std::string& difficulty) 
    : m_people(initialPeople), 
      m_availablePeople(initialPeople),
      m_crop(initialCrop),
      m_gold(initialGold),
      m_weaponLevel(1),
      m_difficulty(difficulty) {}

// 资源增减
void Player::addCrop(int amount) { 
    m_crop = std::max(0, m_crop + amount); 
}

void Player::addGold(int amount) { 
    m_gold = std::max(0, m_gold + amount); 
}

void Player::addPeople(int amount) { 
    m_people = std::max(0, m_people + amount);
    m_availablePeople = std::max(0, m_availablePeople + amount);
}

// 人员分配
void Player::assignWorkers(int farming, int mining, int recruiting, int shopping, int exploring) {
    int total = farming + mining + recruiting + shopping + exploring;
    if (total > m_availablePeople) return;

    m_farmingWorkers = farming;
    m_miningWorkers = mining;
    m_recruitingWorkers = recruiting;
    m_shoppingWorkers = shopping;
    m_exploringWorkers = exploring;
    m_availablePeople -= total;
}

void Player::resetDailyWorkers() {
    m_availablePeople = m_people;
    m_farmingWorkers = m_miningWorkers = m_recruitingWorkers = m_shoppingWorkers = m_exploringWorkers = 0;
}

// 武器升级
bool Player::canUpgradeWeapon() const {
    if (m_weaponLevel >= 10) return false;
    return m_gold >= WEAPON_UPGRADE_COST.at(m_weaponLevel);
}

void Player::upgradeWeapon() {
    if (canUpgradeWeapon()) {
        m_gold -= WEAPON_UPGRADE_COST.at(m_weaponLevel);
        m_weaponLevel++;
    }
}

// 每日食物消耗
void Player::consumeDailyFood() {
    m_crop = std::max(0, m_crop - m_people * GameConfig::FOOD_PER_PERSON);
    if (m_crop == 0) {
        m_people = std::max(0, m_people - 1); // 饥饿减员
    }
}

// Getter实现
int Player::getDifficulty() const { 
    if (m_difficulty == "EASY") return 1;
    if (m_difficulty == "MEDIUM") return 2;
    if (m_difficulty == "HARD") return 3;
    return 2; // 默认返回中等难度
}
int Player::getPeople() const { return m_people; }
int Player::getAvailablePeople() const { return m_availablePeople; }
int Player::getCrop() const { return m_crop; }
int Player::getGold() const { return m_gold; }
int Player::getWeaponLevel() const { return m_weaponLevel; }
int Player::getTotalHP() const { return m_people * GameConfig::BASE_HP; }
int Player::getFarmingWorkers() const { return m_farmingWorkers; }
int Player::getMiningWorkers() const { return m_miningWorkers; }
int Player::getRecruitingWorkers() const { return m_recruitingWorkers; }
int Player::getShoppingWorkers() const { return m_shoppingWorkers; }
int Player::getExploringWorkers() const { return m_exploringWorkers; }
