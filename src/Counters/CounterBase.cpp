#include "CounterBase.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include <iostream>
#include <limits>

const std::string& CounterBase::GetName() const {
    return m_name;
}

void CounterBase::OnExit() {
    // 默认实现为空
}

