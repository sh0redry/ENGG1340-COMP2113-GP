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

void Animation::WeaponFire(int level) {
    std::string weapon;
    switch(level) {
        case 1: weapon = ">"; break;
        case 2: weapon = "=>"; break;
        case 3: weapon = "==>"; break;
        default: weapon = "===>"; break;
    }
    
    for (int i = 0; i < 3; ++i) {
        Terminal::GetInstance().MoveCursor(20 + i, 10);
        Terminal::GetInstance().SetColor(Terminal::Color::Red, Terminal::Color::Default);
        std::cout << weapon;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Terminal::GetInstance().MoveCursor(20 + i, 10);
        std::cout << "   ";
    }
    Terminal::GetInstance().ResetColor();
}

void Animation::ZombieAttack() {
    for (int i = 0; i < 3; ++i) {
        Terminal::GetInstance().MoveCursor(40, 10);
        Terminal::GetInstance().SetColor(Terminal::Color::Green, Terminal::Color::Default);
        std::cout << "Z";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Terminal::GetInstance().MoveCursor(40, 10);
        std::cout << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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