#pragma once

class Weapon {
private:
    int level;
    int damage;
    int multiple;

    // 武器伤害配置
    static const int weaponDamage[10];
    // 武器联装数量配置
    static const int weaponMultiple[10];

public:
    Weapon(int level = 1);
    
    void upgrade();
    bool canUpgrade() const;
    
    int getDamage() const;
    int getMultiple() const;
    int getLevel() const;
    
    static int getMaxLevel();
}; 