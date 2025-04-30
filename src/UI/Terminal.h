#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <vector>
#include <termios.h>
#include <mutex>
#include <memory>

class Terminal {
private:
    static std::unique_ptr<Terminal> instance;  // 使用智能指针
    static std::mutex mutex;

    // 终端状态
    struct termios oldt, newt;
    std::string defaultColor = "\033[0m";
    
    // 回调函数
    using KeyCallback = void(*)();
    static KeyCallback qKeyCallback;
    static KeyCallback hKeyCallback;
    static KeyCallback wKeyCallback;

    // 私有构造函数
    Terminal();
    
    // 禁止拷贝
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    // 声明 unique_ptr 的删除器为友元
    friend struct std::default_delete<Terminal>;

public:
    // 析构函数移到 public
    ~Terminal();

    // 获取单例实例
    static Terminal& GetInstance();
    
    // 设置q键回调函数
    static void SetQKeyCallback(KeyCallback callback);
    
    // 设置和清除h键回调函数
    static void SetHKeyCallback(KeyCallback callback);
    static void ClearHKeyCallback();
    
    // 设置w键回调函数
    static void SetWKeyCallback(KeyCallback callback);
    static void ClearWKeyCallback();
    
    // 显示控制
    void Clear();
    void MoveCursor(int x, int y);
    void HideCursor();
    void ShowCursor();
    
    // 输入处理
    int GetKeyPress();
    char GetYN();
    std::string GetLine(int maxLength = 50);
    int GetInteger();  // 新增函数：只接受整数输入
    bool CheckInput();     // 检查是否有按键输入
    
    // 颜色控制
    enum class Color {
        Black = 0,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        Default = 9
    };

    void SetColor(Color foreground, Color background = Color::Default);
    void ResetColor();

    // 终端尺寸
    struct TerminalSize {
        int width;
        int height;
    };
    TerminalSize GetTerminalSize();

};

#endif // TERMINAL_H 