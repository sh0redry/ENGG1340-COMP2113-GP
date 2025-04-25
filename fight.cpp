#include <iostream>
#include "Combat.h"
#include "CombatResult.h"
#include "Terminal.h"
#include "UI.h"

using namespace std;

int main() {
    // 游戏参数
    const int DIFFICULTY = 1; // 难度系数，1-3
    const int GAME_LEVEL = 1; // 游戏等级，1-5
    const int PEOPLE = 5; // 玩家数量，1-5
    const int WEAPON_LEVEL = 6; // 武器等级

    // 创建战斗实例
    Combat combat(WEAPON_LEVEL, DIFFICULTY, GAME_LEVEL, PEOPLE);
    
    // 运行战斗
    bool victory = combat.run();
    
    // 处理战斗结果
    int currentPeople = PEOPLE;
    CombatResult::applyResult(currentPeople, victory);
    
    // 显示结果
    auto& terminal = Terminal::GetInstance();
    terminal.Clear();
    
    if (victory) {
        UI::ShowInterface("victory.txt");
    } else {
        UI::ShowInterface("defeat.txt");
    }
    
    terminal.MoveCursor(15, 15);
    cout << "Survivors left: " << currentPeople << endl;
    
    return 0;
}
