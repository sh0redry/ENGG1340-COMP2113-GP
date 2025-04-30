#pragma once
#include "../Core/Player.h"
#include <string>
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include <stdexcept>

// 用于中断Counter执行的异常类
class ReturnToHomeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Return to home menu";
    }
};

class CounterBase {
protected:
    Player& m_player;       // 引用玩家对象
    std::string m_name;     // 计数器名称

    // 返回主菜单的函数
    static void returnToHome() {
        throw ReturnToHomeException();
    }

public:
    // 构造函数：必须绑定玩家和名称
    CounterBase(Player& player, const std::string& name)
        : m_player(player), m_name(name) {}
    
    // 虚析构函数：确保派生类正确释放资源
    virtual ~CounterBase() = default;
    
    // 纯虚函数：子类必须实现
    virtual void OnEnter() = 0;    // 进入计数器时的逻辑
    virtual void Process() = 0;    // 核心处理逻辑
    
    // 可选覆盖函数
    virtual void OnExit();         // 退出计数器时的逻辑
    
    // 获取计数器名称
    const std::string& GetName() const;

    // 设置和清除h键回调
    void setupHKeyCallback() {
        Terminal::GetInstance().SetHKeyCallback(returnToHome);
    }
    
    void clearHKeyCallback() {
        Terminal::GetInstance().ClearHKeyCallback();
    }

    // 设置和清除w键回调
    void setupWKeyCallback(void (*callback)()) {
        Terminal::GetInstance().SetWKeyCallback(callback);
    }
    void clearWKeyCallback() {
        Terminal::GetInstance().ClearWKeyCallback();
    }

    // 设置和清除l键回调
    void setupLKeyCallback(void (*callback)()) {
        Terminal::GetInstance().SetLKeyCallback(callback);
    }
    void clearLKeyCallback() {
        Terminal::GetInstance().ClearLKeyCallback();
    }
};