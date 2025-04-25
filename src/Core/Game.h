#pragma once
#include "WeekCycle.h"
#include "Player.h"
#include "../Combat/Combat.h"
#include "../UI/UI.h"
#include "../Counters/CounterBase.h"
#include <memory>

enum class GameState {
    MAIN_MENU,
    DIFFICULTY_SELECT,
    PLAYING,
    COMBAT,
    GAME_OVER
};

class Game {
public:
    Game();
    void run();

private:
    GameState m_state;
    std::unique_ptr<Player> m_player;
    WeekCycle m_weekCycle;
    std::string m_difficulty;

    void initNewGame();
    void processMainMenu();
    void processDifficultySelect();
    void processDay();
    void handleCounterAction();
    void triggerCombat();
    void showEndScreen(bool victory);
};