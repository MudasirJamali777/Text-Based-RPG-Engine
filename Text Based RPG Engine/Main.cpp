#include <iostream>
#include <memory>
#include <vector>
#include "Character.h"
#include "Monster.h"

// Simple Player class
class Player : public Character {
public:
    Player(std::string n) : Character(n, 100, 20) {}
    void attack(Character& target) override {
        std::cout << "\033[1;32m[YOU]\033[0m Slashed " << target.getName() << " for " << damage << " damage!\n";
        target.takeDamage(damage);
    }
};

int main() {
    auto hero = std::make_unique<Player>("MK_Void");
    auto enemy = std::make_unique<Monster>("Glitch_Wraith", 80, 15);

    std::cout << "\033[1;31m=== BATTLE INITIALIZED ===\033[0m\n";

    while (hero->isAlive() && enemy->isAlive()) {
        std::cout << "\n-- " << hero->getName() << ": " << hero->getHealth() << " HP | "
            << enemy->getName() << ": " << enemy->getHealth() << " HP --\n";
        std::cout << "1. Standard Attack\n2. Defend (Incoming)\nChoice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            hero->attack(*enemy);
        }
        else {
            std::cout << "You braced for impact!\n";
            // We can add defense logic tomorrow!
        }

        if (enemy->isAlive()) {
            enemy->attack(*hero);
        }
    }

    if (hero->isAlive()) {
        std::cout << "\n\033[1;32m[SUCCESS]\033[0m Enemy deleted. System stable.\n";
    }
    else {
        std::cout << "\n\033[1;31m[FAILURE]\033[0m Connection lost. Player destroyed.\n";
    }

    return 0;
}