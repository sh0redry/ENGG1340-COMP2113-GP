#include "Game.h"
#include "Difficulty.h"
#include "Counters/CounterFactory.h"
#include "Animation.h"
#include "Terminal.h"
#include "Constants.h"

#include <thread>

Game::Game() : m_state(GameState::MAIN_MENU) {}

void Game::run() {
    while (true) {
        Terminal::Clear();
        
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
        config.initialPeople * 5,  // 初始金币
        m_difficulty              // 传递难度参数
    );
    m_weekCycle = WeekCycle();
    Animation::ProgressBar(2.0f);
}

void Game::processMainMenu() {
    UI::ShowMainMenu();
    int choice = Terminal::GetKeyPress() - '0';
    
    if (choice == 1) m_state = GameState::DIFFICULTY_SELECT;
    else if (choice == 2) m_state = GameState::GAME_OVER;
}

void Game::processDifficultySelect() {
    UI::ShowDifficultyMenu();
    char input = Terminal::GetKeyPress();
    
    if (input == '1') m_difficulty = "EASY";
    else if (input == '2') m_difficulty = "MEDIUM";
    else if (input == '3') m_difficulty = "HARD";
    else return;
    
    initNewGame();
    m_state = GameState::PLAYING;
}

void Game::processDay() {
    // 显示天数过渡
    UI::ShowDayTransition(m_weekCycle.getCurrentDay());
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 每日开始重置
    m_player->resetDailyWorkers();
    m_player->consumeDailyFood();
    
    // 主游戏循环
    while (m_player->getAvailablePeople() > 0) {
        UI::ShowHomeScreen(*m_player, m_weekCycle);
        handleCounterAction();
    }
    
    // 推进到下一天
    m_weekCycle.advanceDay();
    
    // 检查星期四战斗
    if (m_weekCycle.isThursday()) {
        m_state = GameState::COMBAT;
    }
    
    // 检查游戏结束
    if (m_weekCycle.getCurrentDay() > GameConstants::TOTAL_DAYS) {
        showEndScreen(true);
    } else if (m_player->getPeople() <= 0) {
        showEndScreen(false);
    }
}

void Game::handleCounterAction() {
    char input = Terminal::GetKeyPress();
    CounterType type = static_cast<CounterType>(input - '1');
    
    auto counter = CounterFactory::Create(type, *m_player);
    if (!counter) return;
    
    try {
        counter->OnEnter();
        int workers = UI::GetWorkerAllocationInput(m_player->getAvailablePeople());
        counter->Process(workers);
        counter->OnExit();
    } catch (const ReturnToHomeException&) {
        // 捕获返回主菜单的异常，直接返回
        return;
    }
}

void Game::triggerCombat() {
    Combat combat(*m_player, m_weekCycle);
    bool victory = combat.run();
    m_state = victory ? GameState::PLAYING : GameState::GAME_OVER;
}

void Game::showEndScreen(bool victory) {
    UI::ShowGameOver(victory);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    m_state = GameState::GAME_OVER;
}