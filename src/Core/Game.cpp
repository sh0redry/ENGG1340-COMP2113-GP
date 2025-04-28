#include "Game.h"
#include "Difficulty.h"
#include "../Counters/CounterFactory.h"
#include "../UI/Animation.h"
#include "../UI/Terminal.h"
#include "../Utils/Constants.h"
#include "../UI/UI.h"
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
    Animation::ProgressBar(2.0f);
}

void Game::processMainMenu() {
    Animation::PlaySequence("anim/Title", 30);

    int selectedOption = 0; // 0表示Play Game，1表示Quit
    bool selectionMade = false;

    while (!selectionMade) {
        Terminal::GetInstance().Clear();
        
        // 根据当前选择显示对应的界面
        switch (selectedOption) {
            case 0: UI::ShowInterface("menu_play.txt"); break;
            case 1: UI::ShowInterface("menu_quit.txt"); break;
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
            case 0: UI::ShowInterface("difficulty_easy.txt"); break;
            case 1: UI::ShowInterface("difficulty_medium.txt"); break;
            case 2: UI::ShowInterface("difficulty_hard.txt"); break;
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
    // 显示天数过渡（待修改）
    UI::ShowDayTransition(m_weekCycle.getCurrentDay());
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 每日开始重置
    m_player->resetDailyWorkers();
    m_player->consumeDailyFood();
    
    // 主游戏循环
    while (m_player->getAvailablePeople() > 0) {
        handleCounterAction();
    }
    
    // 推进到下一天
    m_weekCycle.advanceDay();
    
    // 检查星期四战斗
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
            case 0: UI::ShowInterface("home_mining.txt"); break;
            case 1: UI::ShowInterface("home_farming.txt"); break;
            case 2: UI::ShowInterface("home_shop.txt"); break;
            case 3: UI::ShowInterface("home_explore.txt"); break;
            case 5: UI::ShowInterface("home_recruit.txt"); break;
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
                else if (selectedOption != 5) selectedOption++; // 其他情况正常右移，但5不能右移
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
    
    auto counter = CounterFactory::Create(type, *m_player);
    if (!counter) return;
    
    try {
        counter->OnEnter();
        counter->Process();
        counter->OnExit();
    } catch (const ReturnToHomeException&) {
        // 捕获返回主菜单的异常，直接返回
        return;
    }
}

void Game::triggerCombat() {
    // 待修改
    Combat combat(*m_player, m_weekCycle);
    bool victory = combat.run();
    m_state = victory ? GameState::PLAYING : GameState::GAME_OVER;
}

void Game::showEndScreen(bool victory) {
    if (victory) {
        Animation::PlaySequence("anim/Win1", 80);
        UI::WaitForEnter("Press Enter to quit...");
        Animation::PlaySequence("anim/Win2", 80);
    } else {
        Animation::PlaySequence("anim/Lose1", 50);
        UI::WaitForEnter("Press Enter to quit...");
        Animation::PlaySequence("anim/Lose2", 100);
    }
    m_state = GameState::GAME_OVER;
}