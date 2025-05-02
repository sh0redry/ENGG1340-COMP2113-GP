#pragma once
#include <map>
#include <string>
#include "../Utils/Constants.h"

/**
 * @class Player
 * @brief Represents a player in the game, managing resources and worker assignments
 * 
 * The Player class handles all player-related operations including:
 * - Resource management (people, crops, gold)
 * - Worker assignments to different tasks
 * - Weapon upgrades
 * - Daily resource consumption
 */
class Player {
private:
    // Basic resources
    int m_people;           // Total population
    int m_availablePeople;  // Available workers for assignment
    int m_crop;             // Food resources
    int m_gold;             // Gold resources
    int m_weaponLevel;      // Weapon level (1-10)
    std::string m_difficulty; // Game difficulty level

    // Worker assignments
    int m_farmingWorkers = 0;    // Workers assigned to farming
    int m_miningWorkers = 0;     // Workers assigned to mining
    int m_recruitingWorkers = 0; // Workers assigned to recruiting
    int m_shoppingWorkers = 0;   // Workers assigned to shopping
    int m_exploringWorkers = 0;  // Workers assigned to exploring

public:
    /**
     * @brief Constructs a new Player object
     * @param initialPeople Initial population count
     * @param initialCrop Initial food resources
     * @param initialGold Initial gold resources
     * @param difficulty Game difficulty level ("EASY", "MEDIUM", "HARD")
     */
    explicit Player(int initialPeople, int initialCrop, int initialGold, const std::string& difficulty = "MEDIUM");

    /**
     * @brief Adds or subtracts crop resources
     * @param amount Amount to add (positive) or subtract (negative)
     */
    void addCrop(int amount);

    /**
     * @brief Adds or subtracts gold resources
     * @param amount Amount to add (positive) or subtract (negative)
     */
    void addGold(int amount);

    /**
     * @brief Adds or subtracts population
     * @param amount Amount to add (positive) or subtract (negative)
     */
    void addPeople(int amount);

    /**
     * @brief Assigns workers to different tasks
     * @param farming Number of workers for farming
     * @param mining Number of workers for mining
     * @param recruiting Number of workers for recruiting
     * @param shopping Number of workers for shopping
     * @param exploring Number of workers for exploring
     * @return void
     * @note Total assigned workers cannot exceed available workers
     */
    void assignWorkers(int farming, int mining, int recruiting, int shopping, int exploring);

    /**
     * @brief Resets all worker assignments and makes all people available
     */
    void resetDailyWorkers();

    /**
     * @brief Upgrades the player's weapon level
     * @note Deducts gold based on current weapon level
     */
    void upgradeWeapon();

    /**
     * @brief Consumes daily food for all people
     * @note If food is insufficient, reduces population by 1
     */
    void consumeDailyFood();

    /**
     * @brief Gets the numerical difficulty level
     * @return 1 for EASY, 2 for MEDIUM, 3 for HARD
     */
    int getDifficulty() const;

    /**
     * @brief Gets the string representation of difficulty
     * @return "EASY", "MEDIUM", or "HARD"
     */
    std::string getStringDifficulty() const;

    /**
     * @brief Gets total population
     * @return Current total population count
     */
    int getPeople() const;

    /**
     * @brief Gets available workers for assignment
     * @return Number of unassigned workers
     */
    int getAvailablePeople() const;

    /**
     * @brief Gets current crop resources
     * @return Current food amount
     */
    int getCrop() const;

    /**
     * @brief Gets current gold resources
     * @return Current gold amount
     */
    int getGold() const;

    /**
     * @brief Gets current weapon level
     * @return Weapon level (1-10)
     */
    int getWeaponLevel() const;

    /**
     * @brief Calculates total HP based on population
     * @return Total HP (population * BASE_HP)
     */
    int getTotalHP() const;

    /**
     * @brief Gets number of farming workers
     * @return Current farming workers count
     */
    int getFarmingWorkers() const;

    /**
     * @brief Gets number of mining workers
     * @return Current mining workers count
     */
    int getMiningWorkers() const;

    /**
     * @brief Gets number of recruiting workers
     * @return Current recruiting workers count
     */
    int getRecruitingWorkers() const;

    /**
     * @brief Gets number of shopping workers
     * @return Current shopping workers count
     */
    int getShoppingWorkers() const;

    /**
     * @brief Gets number of exploring workers
     * @return Current exploring workers count
     */
    int getExploringWorkers() const;

    /**
     * @brief Map of weapon levels to their upgrade costs
     */
    static const std::map<int, int> WEAPON_UPGRADE_COST;
};