#include "Character.h"
#include <iostream>

void Character::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
    std::cout << "\033[1;32m[RECOVERY]\033[0m " << name << " healed for " << amount << ". (HP: " << health << ")\n";
}