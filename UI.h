#ifndef UI_H
#define UI_H

#include <string>
#include "Player.h"

class UI {
public:
    static void ShowInterface(const std::string& filename);
    
    // 游戏状态
    static void ShowDayTransition(int day);
    
    // 工具方法
    static void DrawStatusBar(const Player& player);

    // 新增：在框内移动光标
    static void MoveCursorInBox(int x, int y);

private:
    // UI 辅助函数
    static std::string LoadUI(const std::string& filename);
    static void DisplayUI(const std::string& content);
    static void DisplayUIFromFile(const std::string& filename);

    // 新增：获取UI框坐标
    struct BoxCoordinates {
        int top;
        int bottom;
        int left;
        int right;
    };
    static BoxCoordinates GetBoxCoordinates(const std::string& content);

    // 框的尺寸常量
    static constexpr int BOX_WIDTH = 120;  // 从home.txt中获取的框宽度
    static constexpr int BOX_HEIGHT = 40;  // 从home.txt中获取的框高度
};

#endif // UI_H 