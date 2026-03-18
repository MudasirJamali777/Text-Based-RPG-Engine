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
    int maxHealth; // New
    int damage;

public:
    Character(std::string n, int h, int d) : name(n), health(h), maxHealth(h), damage(d) {}
    virtual ~Character() = default;

    virtual void takeDamage(int amount);
    virtual void attack(Character& target) = 0;

    void heal(int amount); // Added healing logic

    int getHealth() const { return health; }
    std::string getName() const { return name; }
    bool isAlive() const { return health > 0; }
};

#endif