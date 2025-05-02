/**
 * @file Animation.h
 * @brief Animation management class for handling text and sequence animations
 * 
 * This file defines the Animation class, which provides functionality for:
 * - Text-based animations (typewriter effect)
 * - Frame-based sequence animations
 * - Animation frame management and sequencing
 * 
 * The Animation class supports both simple text animations and complex
 * multi-frame sequence animations, with configurable timing and positioning.
 * 
 * @see UI
 * @see Terminal
 */

#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <vector>

class Animation {
public:
    /**
     * @brief Displays text with a typewriter effect
     * 
     * Displays text character by character with a specified delay
     * between characters.
     * @param text The text to animate
     * @param delayMs Delay between characters in milliseconds
     * @return void
     */
    static void Typewriter(const std::string& text, int delayMs = 50);

    /**
     * @brief Displays text with typewriter effect in a UI box
     * 
     * Displays text with typewriter effect at a specific line number
     * within the UI box.
     * @param text The text to animate
     * @param delayMs Delay between characters in milliseconds
     * @param lineNumber The line number to display the text on
     * @return void
     */
    static void TypewriterInBox(const std::string& text, int delayMs, int lineNumber);

    /**
     * @brief Plays a sequence of animation frames
     * 
     * Loads and displays a sequence of animation frames from a directory,
     * with a specified delay between frames.
     * @param dirPath Path to the directory containing animation frames
     * @param frameDelayMs Delay between frames in milliseconds
     * @return void
     */
    static void PlaySequence(const std::string& dirPath, int frameDelayMs);

private:
    /**
     * @brief Gets sorted list of animation frame files
     * 
     * Retrieves and sorts animation frame files from a directory
     * based on their frame numbers.
     * @param dirPath Path to the directory containing animation frames
     * @return std::vector<std::string> containing sorted frame filenames
     */
    static std::vector<std::string> GetSortedFrames(const std::string& dirPath);
    
    /**
     * @brief Extracts frame number from filename
     * 
     * Parses the frame number from an animation frame filename.
     * @param filename The animation frame filename
     * @return int The extracted frame number
     */
    static int ExtractFrameNumber(const std::string& filename);

    /**
     * @brief Compares animation frame filenames
     * 
     * Compares two animation frame filenames based on their frame numbers.
     * @param a First filename to compare
     * @param b Second filename to compare
     * @return bool True if a should come before b in sequence
     */
    static bool CompareAnimationFiles(const std::string& a, const std::string& b);
};