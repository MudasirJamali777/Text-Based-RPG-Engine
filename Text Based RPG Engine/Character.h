#pragma once
#include <string>

class Character {
protected:
    std::string name;
    int health;
    int damage;

public:
    Character(std::string n, int h, int d);
    virtual ~Character() = default;

    virtual void takeDamage(int amount);

    // New: Pure virtual function makes this an "Abstract Class"
    virtual void attack(Character& target) = 0;

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    bool isAlive() const { return health > 0; }
};