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


void Animation::Typewriter(const std::string& text, int delayMs) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void Animation::TypewriterInBox(const std::string& text, int delayMs, int lineNumber) {
    UI::MoveCursorToCenter(text, lineNumber);
    Typewriter(text, delayMs);
}

bool Animation::CompareAnimationFiles(const std::string& a, const std::string& b) {
    return ExtractFrameNumber(a) < ExtractFrameNumber(b);
}

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
