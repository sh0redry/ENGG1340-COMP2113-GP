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


void Animation::ProgressBar(float seconds, int width) {
    Terminal::GetInstance().HideCursor();
    for (int i = 0; i <= width; ++i) {
        Terminal::GetInstance().MoveCursor(10, 15);
        std::cout << "[" << std::string(i, '=') 
                  << std::string(width - i, ' ') << "] " 
                  << int((float)i/width*100) << "%";
        std::this_thread::sleep_for(
            std::chrono::milliseconds(int(seconds*1000/width))
        );
    }
    Terminal::GetInstance().ShowCursor();
}

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

void Animation::PulseText(const std::string& text, int x, int y) {
    Terminal::GetInstance().MoveCursor(x, y);
    for (int i = 0; i < 3; ++i) {
        Terminal::GetInstance().SetColor(Terminal::Color::White, Terminal::Color::Default);
        std::cout << text;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        Terminal::GetInstance().MoveCursor(x, y);
        Terminal::GetInstance().SetColor(Terminal::Color::Default, Terminal::Color::Default);
        std::cout << text;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        Terminal::GetInstance().MoveCursor(x, y);
    }
    Terminal::GetInstance().ResetColor();
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

void Effects::FlashScreen(int times) {
    for (int i = 0; i < times; ++i) {
        Terminal::GetInstance().Clear();
        Terminal::GetInstance().SetColor(Terminal::Color::White, Terminal::Color::Default);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Terminal::GetInstance().Clear();
        Terminal::GetInstance().ResetColor();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
} 