#pragma once
#include "Character.h" 

class Monster : public Character {
public:
    bool isBoss;

    Monster(std::string n, int h, int d, bool boss = false)
        : Character(n, h, d), isBoss(boss) {
    }

    void attack(Character& target) override {
        int damageMultiplier = isBoss ? 1.5 : 1.0; // Bosses deal 50% more damage
        int finalDmg = damage * damageMultiplier;

        std::cout << "\033[1;31m[" << name << "]\033[0m Strikes for " << finalDmg << "!\n";
        target.takeDamage(finalDmg);
    }
};