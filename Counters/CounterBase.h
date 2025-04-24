#pragma once
#include "../Player.h"
#include <string>

class CounterBase {
protected:
    Player& m_player;       // 引用玩家对象
    std::string m_name;     // 计数器名称

public:
    // 构造函数：必须绑定玩家和名称
    CounterBase(Player& player, const std::string& name);
    
    // 虚析构函数：确保派生类正确释放资源
    virtual ~CounterBase() = default;
    
    // 纯虚函数：子类必须实现
    virtual void OnEnter() = 0;    // 进入计数器时的逻辑
    virtual void Process() = 0;    // 核心处理逻辑
    
    // 可选覆盖函数
    virtual void OnExit();         // 退出计数器时的逻辑
    
    // 获取计数器名称
    const std::string& GetName() const;
};