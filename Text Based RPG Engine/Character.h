#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <iostream>

struct Item {
    std::string itemName;
    int healAmount;
};

class Character {
protected:
    std::string name;
    int health;
    int maxHealth;
    int damage;
    int level; // This is the new one for Day 4!

public:
    // Update this line to include lvl = 1
    Character(std::string n, int h, int d, int lvl = 1)
        : name(n), health(h), maxHealth(h), damage(d), level(lvl) {
    }
    virtual ~Character() = default;

    virtual void takeDamage(int amount);
    virtual void attack(Character& target) = 0;

    void heal(int amount); // Added healing logic

    int getHealth() const { return health; }
    std::string getName() const { return name; }
    bool isAlive() const { return health > 0; }
};

#endif