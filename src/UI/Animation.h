#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <chrono>

class Animation {
public:
    // 进度条
    static void ProgressBar(float seconds, int width = 50);
    
    // 文字动画
    static void Typewriter(const std::string& text, int delayMs = 50);
    static void PulseText(const std::string& text, int x, int y);
    
    // 战斗动画
    static void WeaponFire(int level);
    static void ZombieAttack();
};

// 独立动画效果
namespace Effects {
    void FlashScreen(int times);
}

#endif // ANIMATION_H 