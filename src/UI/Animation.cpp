/**
 * @file Animation.cpp
 * @brief Implementation of animation effects and sequence playback
 * @details Handles text animations and frame-based sequence animations
 *          with configurable timing and positioning.
 */
#include "Animation.h"
#include "Terminal.h"
#include "UI.h"
#include <iostream>
#include <thread>
#include <cmath>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <dirent.h>
#include <algorithm>

/**
 * @brief Displays text with typewriter effect
 * 
 * Shows text character by character with specified delay,
 * handling cursor visibility.
 * @param text The text to animate
 * @param delayMs Delay between characters in milliseconds
 */
void Animation::Typewriter(const std::string& text, int delayMs) {
    Terminal::GetInstance().ShowCursor();
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    Terminal::GetInstance().HideCursor();
}

/**
 * @brief Displays text with typewriter effect in UI box
 * 
 * Centers and displays text with typewriter effect at specified line.
 * @param text The text to animate
 * @param delayMs Delay between characters in milliseconds
 * @param lineNumber The line number to display the text on
 */
void Animation::TypewriterInBox(const std::string& text, int delayMs, int lineNumber) {
    UI::MoveCursorToCenter(text, lineNumber);
    Typewriter(text, delayMs);
}

/**
 * @brief Compares animation frame filenames
 * 
 * Compares two animation frame filenames based on their frame numbers
 * for sorting purposes.
 * @param a First filename to compare
 * @param b Second filename to compare
 * @return bool True if a should come before b in sequence
 */
bool Animation::CompareAnimationFiles(const std::string& a, const std::string& b) {
    return ExtractFrameNumber(a) < ExtractFrameNumber(b);
}

/**
 * @brief Extracts frame number from filename
 * 
 * Parses the numeric frame number from an animation frame filename.
 * @param filename The animation frame filename
 * @return int The extracted frame number
 * @throw std::runtime_error if filename format is invalid
 */
int Animation::ExtractFrameNumber(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos || filename.substr(dotPos) != ".txt") {
        throw std::runtime_error("Invalid animation frame format: " + filename);
    }
    
    try {
        return std::stoi(filename.substr(0, dotPos));
    } catch (...) {
        throw std::runtime_error("Failed to parse frame number: " + filename);
    }
}

/**
 * @brief Gets sorted list of animation frame files
 * 
 * Retrieves and sorts animation frame files from a directory
 * based on their frame numbers.
 * @param dirPath Path to the directory containing animation frames
 * @return std::vector<std::string> containing sorted frame filenames
 * @throw std::runtime_error if directory cannot be opened
 */
std::vector<std::string> Animation::GetSortedFrames(const std::string& dirPath) {
    std::vector<std::string> frames;
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) {
        throw std::runtime_error("Cannot open animation directory: " + dirPath);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        if (filename[0] != '.' && filename.find(".txt") != std::string::npos) {
            frames.push_back(filename);
        }
    }
    closedir(dir);
    
    std::sort(frames.begin(), frames.end(), CompareAnimationFiles);
    return frames;
}

/**
 * @brief Plays a sequence of animation frames
 * 
 * Loads and displays a sequence of animation frames with specified delay.
 * @param dirPath Path to the directory containing animation frames
 * @param frameDelayMs Delay between frames in milliseconds
 * @throw std::runtime_error if no frames are found
 */
void Animation::PlaySequence(const std::string& dirPath, int frameDelayMs) {
    auto frames = GetSortedFrames(dirPath);
    if (frames.empty()) {
        throw std::runtime_error("No animation frames found in: " + dirPath);
    }

    for (const auto& frameFile : frames) {
        UI::ShowInterface(dirPath + "/" + frameFile);
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMs));
    }
}
