#pragma once // Prevents the file from being included twice
#include <string>

class Character {
protected:
    std::string name;
    int health;
    int damage;

public:
    Character(std::string n, int h, int d);

    // Virtual means child classes can change how this works
    virtual void takeDamage(int amount);
    int getHealth() const { return health; }
    std::string getName() const { return name; }
    bool isAlive() const { return health > 0; }
};