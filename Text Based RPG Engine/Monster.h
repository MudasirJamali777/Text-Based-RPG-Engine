#pragma once
#include "Character.h" 

class Monster : public Character {
public:
    // This passes the name, health, and damage up to the parent 'Character'
    Monster(std::string n, int h, int d, int lvl = 1) : Character(n, h, d, lvl) {}

    // This tells the game how a Monster attacks specifically
    void attack(Character& target) override {
        int critRoll = rand() % 100 + 1;
        int monsterDmg = damage;

        if (critRoll <= 10) { // 10% chance
            monsterDmg *= 1.5; // Boss-level power!
            std::cout << "\033[1;31m[CRITICAL GLITCH!]\033[0m ";
        }

        std::cout << "\033[1;31m[" << name << "]\033[0m " << name << " glitches through space to hit " << target.getName() << " for " << monsterDmg << "!\n";
        target.takeDamage(monsterDmg);
    }
};