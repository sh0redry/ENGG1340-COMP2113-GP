#include "CounterBase.h"
#include "../Terminal.h"
#include "../UI.h"
#include <iostream>
#include <limits>

CounterBase::CounterBase(Player& player, const std::string& name)
    : m_player(player), m_name(name) {}

const std::string& CounterBase::GetName() const {
    return m_name;
}

void CounterBase::OnExit() {
    // 默认实现为空
}

