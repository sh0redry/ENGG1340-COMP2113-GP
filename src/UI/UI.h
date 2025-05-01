#pragma once
#include <string>

class UI {
public:
    static void ShowInterface(const std::string& filename);
    
    // 游戏状态
    static void ShowDayTransition(std::string dayName, int currentWeek);
    
    // 新增：在框内移动光标
    static void MoveCursorInBox(int x, int y);

    // 新增：等待用户按回车继续
    static void WaitForEnter(const std::string& message = "Press Enter to continue...");

    // 新增：获取UI框坐标
    struct BoxCoordinates {
        int top;
        int bottom;
        int left;
        int right;
    };
    static BoxCoordinates GetBoxCoordinates(const std::string& content);

    // 新增：加载UI文件
    static std::string LoadUI(const std::string& filename);

    // 新增：在框中居中移动光标
    static void MoveCursorToCenter(const std::string& text, int lineNumber);

    // 新增：在框中居中显示文本
    static void DisplayCenterText(const std::string& text, int lineNumber);

    // 框的尺寸常量
    static constexpr int BOX_WIDTH = 122;  // 从empty.txt中获取的框宽度 (120个═ + 2个边框字符)
    static constexpr int BOX_HEIGHT = 40;  // 从empty.txt中获取的框高度


private:
    // UI 辅助函数
    static void DisplayUI(const std::string& content);
    static void DisplayUIFromFile(const std::string& filename);
};