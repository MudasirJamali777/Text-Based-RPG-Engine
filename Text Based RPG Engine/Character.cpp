#include "Character.h"

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    std::cout << "\033[1;31m[DAMAGE]\033[0m " << name << " took " << amount << " damage!\n";
}

void Character::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
    std::cout << "\033[1;32m[HEAL]\033[0m " << name << " restored HP! Current: " << health << "/" << maxHealth << "\n";
}