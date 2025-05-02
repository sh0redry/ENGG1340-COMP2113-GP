#include "Player.h"
#include "Difficulty.h"

/**
 * @brief Weapon upgrade costs for each level
 * Key: Weapon level (1-9)
 * Value: Gold cost for upgrading to next level
 */
const std::map<int, int> Player::WEAPON_UPGRADE_COST = {
    {1, 30}, {2, 30}, {3, 30}, {4, 40}, {5, 40},
    {6, 40}, {7, 50}, {8, 50}, {9, 50}
};

/**
 * @brief Constructs a new Player object with initial resources
 * @param initialPeople Starting population count
 * @param initialCrop Starting food resources
 * @param initialGold Starting gold resources
 * @param difficulty Game difficulty level
 */
Player::Player(int initialPeople, int initialCrop, int initialGold, const std::string& difficulty) 
    : m_people(initialPeople), 
      m_availablePeople(initialPeople),
      m_crop(initialCrop),
      m_gold(initialGold),
      m_weaponLevel(1),
      m_difficulty(difficulty) {}

/**
 * @brief Adds or subtracts crop resources
 * @param amount Amount to add (positive) or subtract (negative)
 * @note Ensures crop amount never goes below 0
 */
void Player::addCrop(int amount) { 
    m_crop = std::max(0, m_crop + amount); 
}

/**
 * @brief Adds or subtracts gold resources
 * @param amount Amount to add (positive) or subtract (negative)
 * @note Ensures gold amount never goes below 0
 */
void Player::addGold(int amount) { 
    m_gold = std::max(0, m_gold + amount); 
}

/**
 * @brief Adds or subtracts population
 * @param amount Amount to add (positive) or subtract (negative)
 * @note Ensures population never goes below 0
 */
void Player::addPeople(int amount) { 
    m_people = std::max(0, m_people + amount);
    m_availablePeople = std::max(0, m_availablePeople + amount);
}

/**
 * @brief Assigns workers to different tasks
 * @param farming Number of workers for farming
 * @param mining Number of workers for mining
 * @param recruiting Number of workers for recruiting
 * @param shopping Number of workers for shopping
 * @param exploring Number of workers for exploring
 * @note If total workers exceed available workers, no assignment is made
 */
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

/**
 * @brief Resets all worker assignments and makes all people available
 * @note Called at the start of each day
 */
void Player::resetDailyWorkers() {
    m_availablePeople = m_people;
    m_farmingWorkers = m_miningWorkers = m_recruitingWorkers = m_shoppingWorkers = m_exploringWorkers = 0;
}

/**
 * @brief Upgrades the player's weapon level
 * @note Deducts gold based on current weapon level from WEAPON_UPGRADE_COST
 */
void Player::upgradeWeapon() {
    m_gold -= WEAPON_UPGRADE_COST.at(m_weaponLevel);
    m_weaponLevel++;
}

/**
 * @brief Consumes daily food for all people
 * @note If food is insufficient, reduces population by 1
 */
void Player::consumeDailyFood() {
    m_crop = std::max(0, m_crop - m_people * GameConfig::FOOD_PER_PERSON);
    if (m_crop == 0) {
        m_people = std::max(0, m_people - 1); // Starvation reduces population
    }
}

/**
 * @brief Gets the numerical difficulty level
 * @return 1 for EASY, 2 for MEDIUM, 3 for HARD
 */
int Player::getDifficulty() const { 
    if (m_difficulty == "EASY") return 1;
    if (m_difficulty == "MEDIUM") return 2;
    if (m_difficulty == "HARD") return 3;
    return 2; // Default to MEDIUM difficulty
}

/**
 * @brief Gets the string representation of difficulty
 * @return "EASY", "MEDIUM", or "HARD"
 */
std::string Player::getStringDifficulty() const { return m_difficulty; }

/**
 * @brief Gets total population
 * @return Current total population count
 */
int Player::getPeople() const { return m_people; }

/**
 * @brief Gets available workers for assignment
 * @return Number of unassigned workers
 */
int Player::getAvailablePeople() const { return m_availablePeople; }

/**
 * @brief Gets current crop resources
 * @return Current food amount
 */
int Player::getCrop() const { return m_crop; }

/**
 * @brief Gets current gold resources
 * @return Current gold amount
 */
int Player::getGold() const { return m_gold; }

/**
 * @brief Gets current weapon level
 * @return Weapon level (1-10)
 */
int Player::getWeaponLevel() const { return m_weaponLevel; }

/**
 * @brief Calculates total HP based on population
 * @return Total HP (population * BASE_HP)
 */
int Player::getTotalHP() const { return m_people * GameConfig::BASE_HP; }

/**
 * @brief Gets number of farming workers
 * @return Current farming workers count
 */
int Player::getFarmingWorkers() const { return m_farmingWorkers; }

/**
 * @brief Gets number of mining workers
 * @return Current mining workers count
 */
int Player::getMiningWorkers() const { return m_miningWorkers; }

/**
 * @brief Gets number of recruiting workers
 * @return Current recruiting workers count
 */
int Player::getRecruitingWorkers() const { return m_recruitingWorkers; }

/**
 * @brief Gets number of shopping workers
 * @return Current shopping workers count
 */
int Player::getShoppingWorkers() const { return m_shoppingWorkers; }

/**
 * @brief Gets number of exploring workers
 * @return Current exploring workers count
 */
int Player::getExploringWorkers() const { return m_exploringWorkers; }
