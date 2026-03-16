#include "Character.h"
#include <iostream>

Character::Character(std::string n, int h, int d)
    : name(n), health(h), damage(d) {
}

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    std::cout << name << " took " << amount << " damage! (HP: " << health << ")\n";
}