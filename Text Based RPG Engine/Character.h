#pragma once
#include <string>
#include <iostream>

class Character {
public:
    std::string name;
    int health, maxHealth, damage, level, armor;

    Character(std::string n, int h, int d)
        : name(n), health(h), maxHealth(h), damage(d), level(1), armor(0) {}

    virtual ~Character() = default;

    virtual void takeDamage(int amount) {
        int reduced = amount * (100 - armor) / 100;
        health -= reduced;
        if (health < 0) health = 0;
        std::cout << "\033[1;31m[DAMAGE]\033[0m " << name << " took " << reduced << " (Blocked " << armor << "%)\n";
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << "\033[1;32m[HEAL]\033[0m System integrity restored.\n";
    }

    bool isAlive() const { return health > 0; }
    virtual void attack(Character& target) = 0;
};