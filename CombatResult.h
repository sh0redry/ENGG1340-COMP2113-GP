#pragma once

class CombatResult {
public:
    static void applyResult(int& people, bool victory);
    
private:
    static int calculateSurvivors(int initialPeople, bool victory);
}; 