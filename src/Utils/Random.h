#pragma once
#include <random> // C++11随机数库

namespace Random {
    // 线程本地随机数引擎（每个线程独立实例）
    static thread_local std::mt19937 generator(std::random_device{}());
    
    // 整数范围随机
    template<typename T>
    T Range(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(generator);
    }
    
    // 浮点数范围随机
    template<typename T>
    T RangeReal(T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(generator);
    }
    
    // 概率判断（如30%几率返回true）
    inline bool Chance(float probability) {
        std::bernoulli_distribution dist(probability);
        return dist(generator);
    }
}