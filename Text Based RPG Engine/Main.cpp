#include <iostream>
#include <vector>
#include <memory>
#include <ctime>   // For random seed
#include <cstdlib> // For rand()
#include "Character.h"
#include "Monster.h"

class Player : public Character {
public:
    std::vector<Item> inventory;

    Player(std::string n) : Character(n, 120, 25) {
        inventory.push_back({ "Starter Stim", 20 });
    }

    void attack(Character& target) override {
        std::cout << "\033[1;34m[PLAYER]\033[0m Striking " << target.getName() << "!\n";
        target.takeDamage(damage);
    }

    void addLoot(Item loot) {
        std::cout << "\033[1;33m[LOOT FOUND]\033[0m You picked up: " << loot.itemName << "!\n";
        inventory.push_back(loot);
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random generator
    auto hero = std::make_unique<Player>("MK_Void");

    // Day 3 Expansion: Fighting multiple waves
    for (int wave = 1; wave <= 2; ++wave) {
        auto enemy = std::make_unique<Monster>("Glitch_Drone_v" + std::to_string(wave), 50 * wave, 10);
        std::cout << "\n--- WAVE " << wave << " START ---\n";

        while (hero->isAlive() && enemy->isAlive()) {
            std::cout << "\nHP: " << hero->getHealth() << " | Items: " << hero->inventory.size() << "\n";
            std::cout << "1. Attack | 2. Use Item\nChoice: ";
            int choice; std::cin >> choice;

            if (choice == 1) hero->attack(*enemy);
            else if (!hero->inventory.empty()) {
                hero->heal(hero->inventory.back().healAmount);
                hero->inventory.pop_back();
            }

            if (enemy->isAlive()) enemy->attack(*hero);
        }

        if (hero->isAlive()) {
            // Random Loot Drop Logic
            int roll = rand() % 2; // 0 or 1
            if (roll == 0) hero->addLoot({ "Heavy Nano-Kit", 40 });
            else hero->addLoot({ "Small Patch", 15 });
        }
        else break;
    }

    std::cout << "\n\033[1;35m--- END OF DAY 3 LOG ---\033[0m\n";
    return 0;
}