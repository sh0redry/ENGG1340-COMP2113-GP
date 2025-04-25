#include "Animation.h"
#include "Terminal.h"
#include <iostream>
#include <thread>
#include <cmath>
#include <chrono>

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