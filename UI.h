#ifndef UI_H
#define UI_H

#include <string>
#include "Player.h"
#include "WeekCycle.h"
#include "Terminal.h"
#include <vector>

class UI {
public:
    // 主界面
    static void ShowMainMenu();
    static void ShowDifficultyMenu();
    static void ShowHomeScreen(const Player& player, const WeekCycle& week);
    
    // 计数器界面
    static void ShowCounterMenu(const std::string& counterName);
    static int GetWorkerAllocationInput(int maxAvailable);
    
    // 战斗界面
    static void ShowCombatScreen(int playerHP, int zombieHP, int weaponLevel);
    static void UpdateCombatScreen(int playerHP, int zombieHP);
    
    // 游戏状态
    static void ShowGameOver(bool victory);
    static void ShowDayTransition(int day);
    
    // 工具方法
    static void DrawStatusBar(const Player& player);
    static void DrawCounterIcons();

    // 输入处理
    static char getKey();
    static int getNumberInput();

private:
    // UI 辅助函数
    static std::string LoadUI(const std::string& filename);
    static void DisplayUI(const std::string& content);
    static void DisplayUIFromFile(const std::string& filename);
};

// 独立工具函数
namespace UIUtils {
    std::string FormatResource(int value);
    void DrawBox(int width, int height);
}

#endif // UI_H 