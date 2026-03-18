#include <iostream>
#include <vector>
#include <memory>
#include "Character.h"
#include "Monster.h"

class Player : public Character {
public:
    std::vector<Item> inventory; // Our Day 3 "Backpack"

    Player(std::string n) : Character(n, 100, 20) {
        // Start with one potion
        inventory.push_back({ "Standard Nano-Kit", 30 });
    }

    void attack(Character& target) override {
        std::cout << "\033[1;32m[YOU]\033[0m Strike! Dealing " << damage << " damage.\n";
        target.takeDamage(damage);
    }

    void useItem() {
        if (!inventory.empty()) {
            Item item = inventory.back(); // Get the last item
            heal(item.healAmount);
            inventory.pop_back(); // Remove it from backpack
        }
        else {
            std::cout << "\033[1;33m[EMPTY]\033[0m No items left in inventory!\n";
        }
    }
};

int main() {
    auto hero = std::make_unique<Player>("MK_Void");
    auto enemy = std::make_unique<Monster>("Glitch_Wraith", 70, 15);

    std::cout << "\033[1;35m--- DAY 3: INVENTORY ACTIVE ---\033[0m\n";

    while (hero->isAlive() && enemy->isAlive()) {
        std::cout << "\n1. Attack | 2. Use Nano-Kit (" << hero->inventory.size() << " left)\nChoice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) hero->attack(*enemy);
        else hero->useItem();

        if (enemy->isAlive()) enemy->attack(*hero);
    }

    return 0;
}