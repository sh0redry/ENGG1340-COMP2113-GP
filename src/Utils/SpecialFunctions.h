#pragma once
#include "../Core/Player.h"
#include "../Core/WeekCycle.h"
#include "../UI/UI.h"
#include <iostream>

class SpecialFunctions {
public:
    static void showWeaponInfo();
    static void showPlayerInfo(const WeekCycle& weekCycle, const Player& player);
    static void showQuitMessage();
}; 