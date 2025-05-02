/**
 * @file CounterBase.cpp
 * @brief Implementation of the base counter class
 * @details Provides the foundation for all game counters with basic
 *          functionality and interface methods that derived counters
 *          can override.
 */
#include "CounterBase.h"
#include "../UI/Terminal.h"
#include "../UI/UI.h"
#include <iostream>
#include <limits>

/**
 * @brief Get the name of the counter
 * @return const reference to the counter's name string
 * 
 * This function returns the name of the counter that was set during construction.
 * The name is used for display purposes and identification.
 */
const std::string& CounterBase::GetName() const {
    return m_name;
}

/**
 * @brief Default implementation of the OnExit function
 * 
 * This is the base implementation of OnExit which does nothing.
 * Derived classes can override this to perform cleanup or state saving
 * when exiting the counter.
 */
void CounterBase::OnExit() {
    // Default implementation is empty
}

