#pragma once
#include <string>
#include <unordered_map>

// 难度配置系统
namespace Difficulty {

    // 难度参数结构体
    struct Config {
        int initialPeople;    // 初始人数
        int cropYield;       // 每人每天种田产量
        int goldYield;       // 每人每天挖矿产量
        float exploreRisk;   // 探索损失概率 (0.05 = 5%)
    };

    // 预设难度配置（EASY/MEDIUM/HARD）
    const std::unordered_map<std::string, Config> PRESETS = {
        { "EASY", 
            Config{ 
                5,  // initialPeople
                40, // cropYield
                40, // goldYield
                0.05f, // exploreRisk
            } 
        },
        { "MEDIUM", 
            Config{ 
                3,  // initialPeople
                30, // cropYield
                30, // goldYield
                0.10f, // exploreRisk
            } 
        },
        { "HARD", 
            Config{ 
                2,  // initialPeople
                20, // cropYield
                20, // goldYield
                0.20f, // exploreRisk
            } 
        }
    };

    // 获取指定难度配置
    inline const Config& GetConfig(const std::string& difficulty) {
        return PRESETS.at(difficulty);
    }

    // 难度验证
    inline bool IsValidDifficulty(const std::string& difficulty) {
        return PRESETS.find(difficulty) != PRESETS.end();
    }
}

