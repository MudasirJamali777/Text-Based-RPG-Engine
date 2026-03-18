#pragma once
#include <string>
#include <vector>

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

public:
    Character(std::string n, int h, int d);
    virtual ~Character() = default;

    virtual void takeDamage(int amount);
    virtual void attack(Character& target) = 0;

    // New for Day 3: Healing logic
    void heal(int amount);

    int getHealth() const { return health; }
    std::string getName() const { return name; }
    bool isAlive() const { return health > 0; }
};