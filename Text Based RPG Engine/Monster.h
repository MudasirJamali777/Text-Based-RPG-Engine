#pragma once
#include "Character.h" 

class Monster : public Character {
public:
    // This passes the name, health, and damage up to the parent 'Character'
    Monster(std::string n, int h, int d, int lvl = 1) : Character(n, h, d, lvl) {}

    // This tells the game how a Monster attacks specifically
    void attack(Character& target) override {
        std::cout << "\033[1;31m[VOID_STRIKE]\033[0m " << name << " glitches through space to hit " << target.getName() << "!\n";
        target.takeDamage(damage);
    }
};