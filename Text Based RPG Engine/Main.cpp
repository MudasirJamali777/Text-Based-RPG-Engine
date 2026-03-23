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

    // --- DAY 7 MERCHANT ACCESSORS ---
    int getXP() const { return xp; }

    bool spendXP(int cost) {
        if (xp >= cost) {
            xp -= cost;
            return true;
        }
        std::cout << "\033[1;31m[MERCHANT]\033[0m You don't have enough XP!\n";
        return false;
    }

    void upgradeDamage(int amount) { damage += amount; }
    void upgradeHealth(int amount) { maxHealth += amount; health = maxHealth; }

    // --- COMBAT LOGIC ---
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

    void equipWeapon(Weapon newWep) {
        std::cout << "\033[1;33m[EQUIP]\033[0m Swapped to: " << newWep.name << " (+" << newWep.damageBonus << " DMG)\n";
        equippedWeapon = newWep;
    }

    void addLoot(Item loot) {
        std::cout << "\033[1;33m[LOOT FOUND]\033[0m You picked up: " << loot.itemName << "!\n";
        inventory.push_back(loot);
    }

    // --- PERSISTENCE ---
    void saveGame() {
        std::ofstream outFile("savegame.txt");
        if (outFile.is_open()) {
            outFile << level << " " << health << " " << maxHealth << " " << damage << " " << xp << " "
                << equippedWeapon.damageBonus << " " << equippedWeapon.name;
            outFile.close();
            std::cout << "\033[1;32m[SYSTEM]\033[0m Game Saved!\n";
        }
    }

    void loadGame() {
        std::ifstream inFile("savegame.txt");
        if (inFile.is_open()) {
            inFile >> level >> health >> maxHealth >> damage >> xp >> equippedWeapon.damageBonus;
            std::getline(inFile >> std::ws, equippedWeapon.name);
            inFile.close();
            std::cout << "\033[1;36m[SYSTEM]\033[0m Game Loaded!\n";
        }
    }

private:
    int xp = 0;
    int xpToNextLevel = 100;
    int mana = 50;
    int maxMana = 50;

public:
    void gainXP(int amount) {
        xp += amount;
        if (xp >= xpToNextLevel) levelUp();
    }

    void levelUp() {
        level++;
        xpToNextLevel = level * 100;
        maxHealth += 20;
        health = maxHealth;
        damage += 5;
        std::cout << "\033[1;33m[LEVEL UP!]\033[0m Reached Level " << level << "!\n";
    }

    void displayHUD() {
        std::cout << "\n========================================";
        std::cout << "\n PLAYER: " << name << " | LVL: " << level << " | WEP: " << equippedWeapon.name;
        std::cout << "\n HP: " << health << "/" << maxHealth << " | MP: " << mana << "/50";
        std::cout << "\n XP: " << xp << "/" << xpToNextLevel;
        std::cout << "\n========================================\n";
    }
};

// --- DAY 7: SAFE ZONE FUNCTION ---
void openShop(Player& hero) {
    std::cout << "\n\033[1;36m--- [SAFE ZONE: THE NEON BAZAAR] ---\033[0m\n";
    std::cout << "Merchant: 'Trade your fragments for upgrades?'\n";

    bool shopping = true;
    while (shopping) {
        std::cout << "\nYour XP: " << hero.getXP() << "\n";
        std::cout << "1. Sharpen Blade (+5 DMG) - 150 XP\n";
        std::cout << "2. Armor Plating (+30 Max HP) - 200 XP\n";
        std::cout << "3. Exit Shop\nChoice: ";

        int choice; std::cin >> choice;
        if (choice == 1 && hero.spendXP(150)) {
            hero.upgradeDamage(5);
            std::cout << "Damage upgraded!\n";
        }
        else if (choice == 2 && hero.spendXP(200)) {
            hero.upgradeHealth(30);
            std::cout << "HP upgraded!\n";
        }
        else {
            shopping = false;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    auto hero = std::make_unique<Player>("MK_Void");
    hero->loadGame();

    int wave = 1;
    while (hero->isAlive()) {
        bool isBossWave = (wave % 5 == 0);
        std::unique_ptr<Monster> enemy;

        if (isBossWave) {
            std::cout << "\n\033[1;35m[!!! BOSS WARNING !!!]\033[0m\n";
            enemy = std::make_unique<Monster>("CORE_OVERLOAD_v" + std::to_string(wave), 200 + (wave * 20), 15 + wave, true);
        }
        else {
            enemy = std::make_unique<Monster>("Glitch_Drone_v" + std::to_string(wave), 40 + (wave * 10), 8 + wave);
        }

        while (hero->isAlive() && enemy->isAlive()) {
            hero->displayHUD();
            std::cout << "\n1. Attack | 2. Heal | 3. Special\nChoice: ";
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

            if (isBossWave) {
                Weapon bossLoot = { "Void-Reaper", 30 + (wave * 5), "LEGENDARY" };
                hero->equipWeapon(bossLoot);
            }
            else {
                hero->gainXP(50 + (wave * 10));
                if (rand() % 2 == 0) hero->addLoot({ "Heavy Nano-Kit", 40 });
                else hero->addLoot({ "Small Patch", 15 });
            }

            hero->saveGame();
            wave++;

            if (wave % 3 == 0 && hero->isAlive()) {
                openShop(*hero);
                hero->saveGame();
            }
        }
        else {
            std::cout << "\033[1;31m[GAME OVER]\033[0m\n";
            break;
        }
    }
    return 0;
}