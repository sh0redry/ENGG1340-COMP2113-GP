#include "Game.h"
#include "Difficulty.h"
#include "../Counters/CounterFactory.h"
#include "../UI/Animation.h"
#include "../UI/Terminal.h"
#include "../Utils/Constants.h"
#include "../UI/UI.h"
#include "../Utils/SpecialFunctions.h"
#include <thread>
#include <iostream>

Game::Game() : m_state(GameState::MAIN_MENU) {}

void Game::run() {
    while (true) {
        Terminal::GetInstance().Clear();
        
        switch (m_state) {
            case GameState::MAIN_MENU: 
                processMainMenu(); 
                break;
            case GameState::DIFFICULTY_SELECT: 
                processDifficultySelect(); 
                break;
            case GameState::PLAYING: 
                processDay(); 
                break;
            case GameState::COMBAT: 
                triggerCombat(); 
                break;
            case GameState::GAME_OVER: 
                return; // 结束游戏
        }
    }
}

void Game::initNewGame() {
    const auto& config = Difficulty::GetConfig(m_difficulty);
    m_player = std::make_unique<Player>(
        config.initialPeople,
        config.initialPeople * 10, // 初始食物
        config.initialPeople * 10,  // 初始金币
        m_difficulty              // 传递难度参数
    );
    m_weekCycle = WeekCycle();
    Animation::PlaySequence("anim/Loading", 70);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Game::processMainMenu() {
    UI::ShowInterface("ui/empty.txt");
    UI::MoveCursorToCenter("Please adjust your terminal size to make the box fit the screen", 18);
    std::cout << "Please adjust your terminal size to make the box fit the screen" << std::endl;
    UI::WaitForEnter("Press Enter to start...");

    Animation::PlaySequence("anim/Title", 50);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    int selectedOption = 0; // 0表示Play Game，1表示Quit
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        // 根据当前选择显示对应的界面
        switch (selectedOption) {
            case 0: UI::ShowInterface("ui/Menu/menu_play.txt"); break;
            case 1: UI::ShowInterface("ui/Menu/menu_quit.txt"); break;
        }
        
        char input = Terminal::GetInstance().GetKeyPress();
        
        switch (input) {
            case 'w':
            case 'W':
                selectedOption = 0;
                break;
            case 's':
            case 'S':
                selectedOption = 1;
                break;
            case '\n': // Enter键
                selectionMade = true;
                break;
        }
    }
    
    if (selectedOption == 0) {
        m_state = GameState::DIFFICULTY_SELECT;
    } else {
        m_state = GameState::GAME_OVER;
    }
}

void Game::processDifficultySelect() {
    int selectedOption = 0; // 0表示EASY，1表示MEDIUM，2表示HARD
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        // 根据当前选择显示对应的界面
        switch (selectedOption) {
            case 0: UI::ShowInterface("ui/Difficulty/difficulty_easy.txt"); break;
            case 1: UI::ShowInterface("ui/Difficulty/difficulty_medium.txt"); break;
            case 2: UI::ShowInterface("ui/Difficulty/difficulty_hard.txt"); break;
        }
                
        char input = Terminal::GetInstance().GetKeyPress();
        
        switch (input) {
            case 'w':
            case 'W':
                if (selectedOption > 0) selectedOption--; // 向上选择，但不允许循环
                break;
            case 's':
            case 'S':
                if (selectedOption < 2) selectedOption++; // 向下选择，但不允许循环
                break;
            case '\n': // Enter键
                selectionMade = true;
                break;
        }
    }
    
    switch (selectedOption) {
        case 0: m_difficulty = "EASY"; break;
        case 1: m_difficulty = "MEDIUM"; break;
        case 2: m_difficulty = "HARD"; break;
    }
    
    initNewGame();
    m_state = GameState::PLAYING;
}

void Game::processDay() {
    // 对第一天需进行特殊处理（增加故事情节）
    if (m_weekCycle.getCurrentDay() == 1) {
        UI::ShowInterface("ui/empty.txt");
        Animation::TypewriterInBox("The year is 2025. ", 50, 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("A zombie outbreak has ravaged HKU, and you must lead a group of survivors ", 50, 13);
        Animation::TypewriterInBox("to gather resources, fortify defenses, and endure relentless Thursday hordes. ", 50, 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Every day brings critical choices ———— shop for upgrades, farm for food, ", 50, 18);
        Animation::TypewriterInBox("mine for gold, recruiting center, or risk expeditions into the wasteland. ", 50, 20);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Will you last long enough for rescue... or fall to the undead?", 100, 23);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        UI::WaitForEnter("Press Enter to continue...");
        UI::ShowInterface("ui/empty.txt");
        Animation::TypewriterInBox("Now, let's start the first day.", 50, 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        Animation::TypewriterInBox("Today is ......", 50, 17);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        UI::ShowInterface("ui/Days/Friday1.txt");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        UI::WaitForEnter("Press Enter to continue...");
    } else {
        UI::ShowDayTransition(m_weekCycle.getDayName(), m_weekCycle.getCurrentWeek());
    }

    // 每日开始重置
    m_player->resetDailyWorkers();
    m_player->consumeDailyFood();
    
    // 主游戏循环
    while (m_player->getAvailablePeople() > 0) {
        handleCounterAction();
    }
    
    // 推进到下一天
    m_weekCycle.advanceDay();
    
    // 检查明天是否是星期四
    if (m_weekCycle.isThursday()) {
        m_state = GameState::COMBAT;
    }
    
    // 检查游戏结束
    if (m_weekCycle.getCurrentDay() > GameConfig::TOTAL_DAYS) {
        showEndScreen(true);
    } else if (m_player->getPeople() <= 0) {
        showEndScreen(false);
    }
}

void Game::handleCounterAction() {
    // 2x3 grid layout for counters
    // [0] [1] [2]
    // [3] [ ] [5]
    int selectedOption = 0; // 0-3,5 for the five counters
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        // 根据当前选择显示对应的界面
        switch (selectedOption) {
            case 0: UI::ShowInterface("ui/Home/home_mining.txt"); break;
            case 1: UI::ShowInterface("ui/Home/home_farming.txt"); break;
            case 2: UI::ShowInterface("ui/Home/home_shop.txt"); break;
            case 3: UI::ShowInterface("ui/Home/home_explore.txt"); break;
            case 5: UI::ShowInterface("ui/Home/home_recruit.txt"); break;
        }
                
        char input = Terminal::GetInstance().GetKeyPress();
        
        switch (input) {
            case 'w':
            case 'W':
                if (selectedOption > 2) selectedOption -= 3; // 向上移动
                break;
            case 's':
            case 'S':
                if (selectedOption < 3 && selectedOption != 1) selectedOption += 3; // 向下移动，但1不能下移
                break;
            case 'a':
            case 'A':
                if (selectedOption > 0) {
                    if (selectedOption == 5) selectedOption = 3; // 从5直接到3
                    else if (selectedOption != 3) selectedOption--; // 其他情况正常左移，但3不能左移
                }
                break;
            case 'd':
            case 'D':
                if (selectedOption < 2) selectedOption++; // 第一行正常右移
                else if (selectedOption == 3) selectedOption = 5; // 从3直接到5
                else if (selectedOption != 5 && selectedOption != 2) selectedOption++; // 其他情况正常右移，但5和2不能右移
                break;
            case 'l':
            case 'L':
                SpecialFunctions::showPlayerInfo(m_weekCycle, *m_player);
                break;
            case 'q':
            case 'Q':
                SpecialFunctions::showQuitMessage();
                break;
            case '\n': // Enter键
                selectionMade = true;
                break;
        }
    }
    
    // 将选择的选项转换为对应的CounterType
    CounterType type;
    switch (selectedOption) {
        case 0: type = CounterType::MINING; break;
        case 1: type = CounterType::FARMING; break;
        case 2: type = CounterType::SHOP; break;
        case 3: type = CounterType::EXPLORE; break;
        case 5: type = CounterType::RECRUIT; break;
        default: return;
    }
    
    auto counter = CounterFactory::Create(type, *m_player, m_weekCycle);
    if (!counter) return;
    
    try {
        counter->OnEnter();
        counter->Process();
        counter->OnExit();
    } catch (const ReturnToHomeException&) {
        // 捕获返回主菜单的异常，确保调用OnExit
        counter->OnExit();
        return;
    }
}

void Game::triggerCombat() {
    UI::ShowDayTransition(m_weekCycle.getDayName(), m_weekCycle.getCurrentWeek());

    UI::ShowInterface("ui/empty.txt");
    Animation::TypewriterInBox("You are under attack by a horde of zombies! ! !", 50, 13);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("You must defend your home! ! !", 50, 15);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Animation::TypewriterInBox("The horde is approaching ......", 100, 17);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    UI::WaitForEnter("Press Enter to continue...");

    Animation::PlaySequence("anim/Protect", 100);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    Animation::PlaySequence("anim/Fight", 150);
    UI::WaitForEnter("Press Enter to start fight...");

    Combat combat(*m_player, m_weekCycle);
    bool victory = combat.run();
    if (victory) {
        m_weekCycle.advanceDay();  // 战斗胜利后推进到下一天
        m_state = GameState::PLAYING;
    } else {
        m_state = GameState::GAME_OVER;
    }
}

void Game::showEndScreen(bool victory) {
    if (victory) {
        Animation::PlaySequence("anim/Win1", 80);
        UI::WaitForEnter("Press Enter to quit...");
        Animation::PlaySequence("anim/Win2", 100);
    } else {
        Animation::PlaySequence("anim/Lose1", 80);
        UI::WaitForEnter("Press Enter to quit...");
        Animation::PlaySequence("anim/Lose2", 100);
    }
    m_state = GameState::GAME_OVER;
}