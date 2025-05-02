#pragma once
#include <string>
#include <unordered_map>

/**
 * @brief Difficulty configuration system
 * 
 * This namespace manages the game's difficulty settings and provides
 * access to difficulty-specific configurations. It includes preset
 * configurations for different difficulty levels (EASY, MEDIUM, HARD)
 * and utility functions to access and validate difficulty settings.
 */
namespace Difficulty {

    /**
     * @brief Structure containing difficulty-specific configuration parameters
     * 
     * Defines the core parameters that vary with difficulty level:
     * - initialPeople: Starting number of people in the player's team
     * - cropYield: Daily crop production per farmer
     * - goldYield: Daily gold production per miner
     * - exploreRisk: Probability of losing people during exploration (0.05 = 5%)
     */
    struct Config {
        int initialPeople;    // Starting number of people
        int cropYield;       // Daily crop production per farmer
        int goldYield;       // Daily gold production per miner
        float exploreRisk;   // Probability of losing people during exploration (0.05 = 5%)
    };

    /**
     * @brief Preset difficulty configurations
     * 
     * Defines three difficulty levels with their respective configurations:
     * - EASY: Higher starting resources, higher yields, lower risks
     * - MEDIUM: Balanced starting resources, moderate yields, moderate risks
     * - HARD: Lower starting resources, lower yields, higher risks
     */
    const std::unordered_map<std::string, Config> PRESETS = {
        { "EASY", 
            Config{ 
                5,  // initialPeople: Start with 5 people
                40, // cropYield: 40 crops per farmer per day
                40, // goldYield: 40 gold per miner per day
                0.05f, // exploreRisk: 5% chance of losing people during exploration
            } 
        },
        { "MEDIUM", 
            Config{ 
                3,  // initialPeople: Start with 3 people
                30, // cropYield: 30 crops per farmer per day
                30, // goldYield: 30 gold per miner per day
                0.10f, // exploreRisk: 10% chance of losing people during exploration
            } 
        },
        { "HARD", 
            Config{ 
                2,  // initialPeople: Start with 2 people
                20, // cropYield: 20 crops per farmer per day
                20, // goldYield: 20 gold per miner per day
                0.20f, // exploreRisk: 20% chance of losing people during exploration
            } 
        }
    };

    /**
     * @brief Get the configuration for a specific difficulty level
     * @param difficulty The difficulty level to get configuration for
     * @return Reference to the configuration for the specified difficulty
     * @throws std::out_of_range if the difficulty level is invalid
     * 
     * Returns the configuration settings for the specified difficulty level.
     * Throws an exception if the difficulty level is not found in the presets.
     */
    inline const Config& GetConfig(const std::string& difficulty) {
        return PRESETS.at(difficulty);
    }

    /**
     * @brief Check if a difficulty level is valid
     * @param difficulty The difficulty level to validate
     * @return true if the difficulty level exists, false otherwise
     * 
     * Validates whether the specified difficulty level exists in the presets.
     */
    inline bool IsValidDifficulty(const std::string& difficulty) {
        return PRESETS.find(difficulty) != PRESETS.end();
    }
}

