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
        float zombieDamageMultiplier; // 僵尸伤害倍率
    };

    // 预设难度配置（EASY/MEDIUM/HARD）
    const std::unordered_map<std::string, Config> PRESETS = {
        { "EASY", 
            { 
                .initialPeople = 5,
                .cropYield = 20,
                .goldYield = 20,
                .exploreRisk = 0.05f,
                .zombieDamageMultiplier = 0.8f 
            } 
        },
        { "MEDIUM", 
            { 
                .initialPeople = 3,
                .cropYield = 30,
                .goldYield = 30,
                .exploreRisk = 0.10f,
                .zombieDamageMultiplier = 1.0f 
            } 
        },
        { "HARD", 
            { 
                .initialPeople = 2,
                .cropYield = 40,
                .goldYield = 40,
                .exploreRisk = 0.20f,
                .zombieDamageMultiplier = 1.2f 
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

