#include <iostream>
#include <fstream> // For file handling
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
        // Generate a random number between 1 and 100
        int critChance = rand() % 100 + 1;
        int finalDamage = damage;

        if (critChance <= 20) { // 20% chance to crit
            finalDamage *= 2;
            std::cout << "\033[1;33m[CRITICAL HIT!]\033[0m ";
        }

        std::cout << "\033[1;32m[YOU]\033[0m Strike! Dealing " << finalDamage << " damage.\n";
        target.takeDamage(finalDamage);
    }

    void addLoot(Item loot) {
        std::cout << "\033[1;33m[LOOT FOUND]\033[0m You picked up: " << loot.itemName << "!\n";
        inventory.push_back(loot);
    }

    void saveGame() {
        std::ofstream outFile("savegame.txt");
        if (outFile.is_open()) {
            outFile << level << " " << health << " " << maxHealth << " " << damage << " " << xp;
            outFile.close();
            std::cout << "\033[1;32m[SYSTEM]\033[0m Game Saved to savegame.txt\n";
        }
    }

    void loadGame() {
        std::ifstream inFile("savegame.txt");
        if (inFile.is_open()) {
            inFile >> level >> health >> maxHealth >> damage >> xp;
            inFile.close();
            std::cout << "\033[1;36m[SYSTEM]\033[0m Game Loaded! Welcome back, " << name << ".\n";
        }
        else {
            std::cout << "[SYSTEM] No save file found. Starting fresh.\n";
        }
    }

private:
    int xp = 0;
    int xpToNextLevel = 100;

public:
    void gainXP(int amount, int wave = 1) {
        xp += amount;
        std::cout << "\n\033[1;34m[SYSTEM]\033[0m Gained " << amount << " XP! (" << xp << "/" << xpToNextLevel << ")\n";

        if (xp >= xpToNextLevel) {
            levelUp(wave);
        }
    }

    void levelUp(int wave = 1) {
        level++;
        xp = 0;
        xpToNextLevel = level * 100;
        maxHealth += 20;
        health = maxHealth; // Heal to full on level up
        damage += 5;
        std::cout << "\033[1;33m[LEVEL UP!]\033[0m Reached Level " << level << "!\n";
        std::cout << "HP increased to " << maxHealth << " | Damage increased to " << damage << "\n";

        if (isAlive()) {
            std::cout << "\033[1;32m[WAVE CLEAR]\033[0m\n";
            gainXP(50 * wave); // Self XP gain

            int roll = rand() % 2;
            if (roll == 0) addLoot({ "Heavy Nano-Kit", 40 });
            else addLoot({ "Small Patch", 15 });
        }
    }

private:
    int mana = 50;
    int maxMana = 50;

public:
    void specialAbility(Character& target) {
        if (mana >= 20) {
            mana -= 20;
            int specialDmg = damage * 3; // 3x Damage!
            std::cout << "\033[1;35m[ULTIMATE]\033[0m MK_Void unleashes a Glitch-Blast for " << specialDmg << " damage!\n";
            target.takeDamage(specialDmg);
        }
        else {
            std::cout << "\033[1;33m[LOW MANA]\033[0m Not enough energy for a Special Ability!\n";
        }
    }

    // Update your HUD to show Mana
    void displayHUD() {
        std::cout << "\n========================================";
        std::cout << "\n PLAYER: " << name << " | LVL: " << level;
        std::cout << "\n HP: " << health << "/" << maxHealth << " | MP: " << mana << "/" << maxMana;
        std::cout << "\n XP: " << xp << "/" << xpToNextLevel;
        std::cout << "\n========================================\n";
    }
};


int main() {
    srand(static_cast<unsigned int>(time(0)));
    auto hero = std::make_unique<Player>("MK_Void");

    // --- DAY 5: LOAD DATA ---
    // This looks for 'savegame.txt' and updates your level/HP/XP
    hero->loadGame();

    int wave = 1;
    while (hero->isAlive()) {
        auto enemy = std::make_unique<Monster>("Glitch_Drone_v" + std::to_string(wave), 40 + (wave * 10), 8 + wave);
        std::cout << "\n--- ENCOUNTER: WAVE " << wave << " ---\n";

        while (hero->isAlive() && enemy->isAlive()) {
            hero->displayHUD();
            // Added Option 3 here:
            std::cout << "\n1. Attack | 2. Heal | 3. Special Ability (20 MP)\nChoice: ";
            int choice; std::cin >> choice;

            if (choice == 1) {
                hero->attack(*enemy);
            }
            else if (choice == 2 && !hero->inventory.empty()) {
                hero->heal(hero->inventory.back().healAmount);
                hero->inventory.pop_back();
            }
            // Add this else-if block:
            else if (choice == 3) {
                hero->specialAbility(*enemy);
            }

            if (enemy->isAlive()) enemy->attack(*hero);
        }

        if (hero->isAlive()) {
            std::cout << "\033[1;32m[WAVE CLEAR]\033[0m\n";
            hero->gainXP(50 + (wave * 10));

            // --- DAY 5: SAVE DATA ---
            // This writes your new Level and XP to the file after every victory
            hero->saveGame();

            wave++;
        }
        else {
            std::cout << "\033[1;31m[GAME OVER]\033[0m Your journey ends here...\n";
            break;
        }
    }
    return 0;
}