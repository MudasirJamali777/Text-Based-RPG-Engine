#include <iostream>
#include <fstream> 
#include <vector>
#include <memory>
#include <ctime>   
#include <cstdlib> 
#include <string>
#include "Character.h"
#include "Monster.h"
#include "Weapon.h"

class Player : public Character {
public:
    std::vector<Item> inventory;
    Weapon equippedWeapon = { "Rusty Pipe", 5, "Common" };

    Player(std::string n) : Character(n, 120, 25) {
        inventory.push_back({ "Starter Stim", 20 });
    }

    // --- DAY 6: WEAPON ATTACK ---
    void attack(Character& target) override {
        int critChance = rand() % 100 + 1;
        int finalDamage = damage + equippedWeapon.damageBonus;

        if (critChance <= 20) {
            finalDamage *= 2;
            std::cout << "\033[1;33m[CRITICAL HIT!]\033[0m ";
        }

        std::cout << "\033[1;32m[YOU]\033[0m Strike with " << equippedWeapon.name
            << " dealing " << finalDamage << " damage!\n";
        target.takeDamage(finalDamage);
    }

    void equipWeapon(Weapon newWep) {
        std::cout << "\033[1;33m[EQUIP]\033[0m Swapped to: " << newWep.name << " (+" << newWep.damageBonus << " DMG)\n";
        equippedWeapon = newWep;
    }

    void addLoot(Item loot) {
        std::cout << "\033[1;33m[LOOT FOUND]\033[0m You picked up: " << loot.itemName << "!\n";
        inventory.push_back(loot);
    }

    // --- PERSISTENCE LAYER ---
    void saveGame() {
        std::ofstream outFile("savegame.txt");
        if (outFile.is_open()) {
            outFile << level << " " << health << " " << maxHealth << " " << damage << " " << xp << " "
                << equippedWeapon.damageBonus << " " << equippedWeapon.name;
            outFile.close();
            std::cout << "\033[1;32m[SYSTEM]\033[0m Game Saved to savegame.txt\n";
        }
    }

    void loadGame() {
        std::ifstream inFile("savegame.txt");
        if (inFile.is_open()) {
            inFile >> level >> health >> maxHealth >> damage >> xp >> equippedWeapon.damageBonus;
            std::getline(inFile >> std::ws, equippedWeapon.name);
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
    int mana = 50;
    int maxMana = 50;

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
        health = maxHealth;
        damage += 5;
        std::cout << "\033[1;33m[LEVEL UP!]\033[0m Reached Level " << level << "!\n";
        std::cout << "HP increased to " << maxHealth << " | Damage increased to " << damage << "\n";
    }

    void specialAbility(Character& target) {
        if (mana >= 20) {
            mana -= 20;
            int specialDmg = (damage + equippedWeapon.damageBonus) * 3;
            std::cout << "\033[1;35m[ULTIMATE]\033[0m " << name << " unleashes a Glitch-Blast for " << specialDmg << " damage!\n";
            target.takeDamage(specialDmg);
        }
        else {
            std::cout << "\033[1;33m[LOW MANA]\033[0m Not enough energy!\n";
        }
    }

    void displayHUD() {
        std::cout << "\n========================================";
        std::cout << "\n PLAYER: " << name << " | LVL: " << level << " | WEP: " << equippedWeapon.name;
        std::cout << "\n HP: " << health << "/" << maxHealth << " | MP: " << mana << "/" << maxMana;
        std::cout << "\n XP: " << xp << "/" << xpToNextLevel;
        std::cout << "\n========================================\n";
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    auto hero = std::make_unique<Player>("MK_Void");
    hero->loadGame();

    int wave = 1;
    while (hero->isAlive()) {
        bool isBossWave = (wave % 5 == 0);
        std::unique_ptr<Monster> enemy;

        if (isBossWave) {
            std::cout << "\n\033[1;35m[!!! WARNING: VOID ANOMALY DETECTED !!!]\033[0m\n";
            enemy = std::make_unique<Monster>("CORE_OVERLOAD_v" + std::to_string(wave), 200 + (wave * 20), 15 + wave, true);
        }
        else {
            enemy = std::make_unique<Monster>("Glitch_Drone_v" + std::to_string(wave), 40 + (wave * 10), 8 + wave);
        }

        std::cout << "\n--- ENCOUNTER: WAVE " << wave << " START ---\n";

        while (hero->isAlive() && enemy->isAlive()) {
            hero->displayHUD();
            std::cout << "\n1. Attack | 2. Heal | 3. Special Ability (20 MP)\nChoice: ";
            int choice; std::cin >> choice;

            if (choice == 1) hero->attack(*enemy);
            else if (choice == 2 && !hero->inventory.empty()) {
                hero->heal(hero->inventory.back().healAmount);
                hero->inventory.pop_back();
            }
            else if (choice == 3) hero->specialAbility(*enemy);

            if (enemy->isAlive()) enemy->attack(*hero);
        }

        if (hero->isAlive()) {
            std::cout << "\033[1;32m[WAVE CLEAR]\033[0m\n";

            // --- DAY 6 LOOT LOGIC ---
            if (isBossWave) {
                Weapon bossLoot = { "Void-Reaper", 30 + (wave * 5), "LEGENDARY" };
                hero->equipWeapon(bossLoot);
            }
            else {
                hero->gainXP(50 + (wave * 10), wave);
                int roll = rand() % 2;
                if (roll == 0) hero->addLoot({ "Heavy Nano-Kit", 40 });
                else hero->addLoot({ "Small Patch", 15 });
            }

            hero->saveGame();
            wave++;
        }
        else {
            std::cout << "\033[1;31m[GAME OVER]\033[0m Your journey ends at Wave " << wave << ".\n";
            break;
        }
    }
    return 0;
}