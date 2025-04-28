#include "Core/Game.h"
#include "UI/Terminal.h"
#include "Utils/Random.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 初始化系统
    Terminal::GetInstance().HideCursor();
    
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        Terminal::GetInstance().Clear();
        Terminal::GetInstance().MoveCursor(10, 10);
        std::cerr << "Game crashed: " << e.what();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    Terminal::GetInstance().ShowCursor();
    return 0;
}