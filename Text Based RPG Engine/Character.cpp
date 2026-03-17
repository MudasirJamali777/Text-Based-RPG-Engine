#include "Character.h"
#include <iostream>

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;

    // Using \033[1;31m for Bold Red text (Aesthetic)
    std::cout << "\033[1;31m[!] " << name << " took " << amount << " damage!\033[0m" << std::endl;
}