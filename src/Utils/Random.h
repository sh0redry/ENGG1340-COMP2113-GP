#pragma once
#include <random> // C++11 random number library

/**
 * @brief Random number generation system
 * 
 * This namespace provides a thread-safe random number generation system
 * with various utility functions for generating random numbers and
 * probability-based decisions. It uses C++11's random number facilities
 * with a thread-local random number generator to ensure thread safety.
 */
namespace Random {
    /**
     * @brief Thread-local random number generator
     * 
     * Each thread gets its own instance of the random number generator
     * to ensure thread safety and prevent contention between threads.
     */
    static thread_local std::mt19937 generator(std::random_device{}());
    
    /**
     * @brief Generate a random integer within a specified range
     * @tparam T The integer type (e.g., int, long)
     * @param min The minimum value (inclusive)
     * @param max The maximum value (inclusive)
     * @return A random integer between min and max
     * 
     * Generates a random integer value within the specified range [min, max].
     * The range is inclusive of both min and max values.
     */
    template<typename T>
    T Range(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(generator);
    }
    
    /**
     * @brief Generate a random floating-point number within a specified range
     * @tparam T The floating-point type (e.g., float, double)
     * @param min The minimum value (inclusive)
     * @param max The maximum value (exclusive)
     * @return A random floating-point number between min and max
     * 
     * Generates a random floating-point value within the specified range [min, max).
     * The range is inclusive of min but exclusive of max.
     */
    template<typename T>
    T RangeReal(T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(generator);
    }
    
    /**
     * @brief Perform a probability-based decision
     * @param probability The probability of returning true (0.0 to 1.0)
     * @return true with the specified probability, false otherwise
     * 
     * Makes a random decision based on the given probability.
     * For example, Chance(0.3) has a 30% chance of returning true.
     */
    inline bool Chance(float probability) {
        std::bernoulli_distribution dist(probability);
        return dist(generator);
    }
}