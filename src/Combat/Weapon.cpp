#include "Weapon.h"

const int Weapon::weaponDamage[10] = {20, 30, 40, 30, 35, 40, 50, 40, 50, 60};
const int Weapon::weaponMultiple[10] = {1, 1, 1, 3, 3, 3, 3, 5, 5, 5};

Weapon::Weapon(int level) : level(level) {
    damage = weaponDamage[level - 1];
    multiple = weaponMultiple[level - 1];
}

void Weapon::upgrade() {
    if (canUpgrade()) {
        level++;
        damage = weaponDamage[level - 1];
        multiple = weaponMultiple[level - 1];
    }
}

bool Weapon::canUpgrade() const {
    return level < getMaxLevel();
}

int Weapon::getDamage() const {
    return damage;
}

int Weapon::getMultiple() const {
    return multiple;
}

int Weapon::getLevel() const {
    return level;
}

int Weapon::getMaxLevel() {
    return 10;
} 