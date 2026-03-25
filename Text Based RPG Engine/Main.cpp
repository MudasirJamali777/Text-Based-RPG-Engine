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

using namespace std;

class Player : public Character {
public:
    vector<Item> inventory;
    Weapon equippedWeapon = { "Rusty Pipe", 5, "Common" };

    Player(string n) : Character(n, 120, 25) {
        inventory.push_back({ "Starter Stim", 20 });
    }

    int getXP() const { return xp; }

    bool spendXP(int cost) {
        if (xp >= cost) {
            xp -= cost;
            return true;
        }
        cout << "\033[1;31m[MERCHANT]\033[0m You don't have enough XP!\n";
        return false;
    }

    void upgradeDamage(int amount) { damage += amount; }
    void upgradeHealth(int amount) { maxHealth += amount; health = maxHealth; }

    // --- YOUR ORIGINAL COMBAT LOGIC ---
    void attack(Character& target) override {
        int critChance = rand() % 100 + 1;
        int finalDamage = damage + equippedWeapon.damageBonus;

        if (critChance <= 20) {
            finalDamage *= 2;
            cout << "\033[1;33m[CRITICAL HIT!]\033[0m ";
        }

        cout << "\033[1;32m[YOU]\033[0m Strike with " << equippedWeapon.name
            << " dealing " << finalDamage << " damage!\n";
        target.takeDamage(finalDamage);
    }

    // --- YOUR ORIGINAL SPECIAL ABILITY ---
    void specialAbility(Character& target) {
        if (mana >= 20) {
            mana -= 20;
            int specialDmg = (damage + equippedWeapon.damageBonus) * 3;
            cout << "\033[1;35m[ULTIMATE]\033[0m " << name << " unleashes a Glitch-Blast for " << specialDmg << " damage!\n";
            target.takeDamage(specialDmg);
        }
        else {
            cout << "\033[1;33m[LOW MANA]\033[0m Not enough energy!\n";
        }
    }

    void equipWeapon(Weapon newWep) {
        cout << "\033[1;33m[EQUIP]\033[0m Swapped to: " << newWep.name << " (+" << newWep.damageBonus << " DMG)\n";
        equippedWeapon = newWep;
    }

    void addLoot(Item loot) {
        cout << "\033[1;33m[LOOT FOUND]\033[0m You picked up: " << loot.itemName << "!\n";
        inventory.push_back(loot);
    }

    // --- PERSISTENCE (NOW SAVES ARMOR AND MANA) ---
    void saveGame() {
        ofstream outFile("savegame.txt");
        if (outFile.is_open()) {
            outFile << level << " " << health << " " << maxHealth << " " << damage << " " << xp << " "
                << mana << " " << maxMana << " " << armor << " " << equippedWeapon.damageBonus << " " << equippedWeapon.name;
            outFile.close();
            cout << "\033[1;32m[SYSTEM]\033[0m Game Saved!\n";
        }
    }

    void loadGame() {
        ifstream inFile("savegame.txt");
        if (inFile.is_open()) {
            inFile >> level >> health >> maxHealth >> damage >> xp >> mana >> maxMana >> armor >> equippedWeapon.damageBonus;
            getline(inFile >> ws, equippedWeapon.name);
            inFile.close();
            cout << "\033[1;36m[SYSTEM]\033[0m Game Loaded!\n";
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
        mana = maxMana; // Restore mana on level up
        cout << "\033[1;33m[LEVEL UP!]\033[0m Reached Level " << level << "!\n";
    }

    // --- YOUR FULL HUD PLUS ARMOR ---
    void displayHUD() {
        cout << "\n========================================";
        cout << "\n PLAYER: " << name << " | LVL: " << level << " | WEP: " << equippedWeapon.name;
        cout << "\n HP: " << health << "/" << maxHealth << " | MP: " << mana << "/" << maxMana;
        cout << "\n XP: " << xp << "/" << xpToNextLevel << " | ARMOR: " << armor << "%";
        cout << "\n========================================\n";
    }
};

// --- SAFE ZONE FUNCTION ---
void openShop(Player& hero) {
    cout << "\n\033[1;36m--- [SAFE ZONE: THE NEON BAZAAR] ---\033[0m\n";
    cout << "Merchant: 'Trade your fragments for upgrades?'\n";

    bool shopping = true;
    while (shopping) {
        cout << "\nYour XP: " << hero.getXP() << " | Armor: " << hero.armor << "%\n";
        cout << "1. Sharpen Blade (+5 DMG) - 150 XP\n";
        cout << "2. Armor Plating (+10% Def) - 200 XP\n";
        cout << "3. Exit Shop\nChoice: ";

        int choice;
        cin >> choice;
        if (choice == 1 && hero.spendXP(150)) {
            hero.upgradeDamage(5);
            cout << "Damage upgraded!\n";
        }
        else if (choice == 2 && hero.spendXP(200)) {
            if (hero.armor < 50) {
                hero.armor += 10;
                cout << "Armor reinforced!\n";
            }
            else cout << "Max Armor reached!\n";
        }
        else {
            shopping = false;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    auto hero = make_unique<Player>("MK_Void");
    hero->loadGame();

    int wave = 1;
    while (hero->isAlive()) {
        bool isBossWave = (wave % 5 == 0);
        unique_ptr<Monster> enemy;

        // --- YOUR BOSS LOGIC ---
        if (isBossWave) {
            cout << "\n\033[1;35m[!!! BOSS WARNING !!!]\033[0m\n";
            enemy = make_unique<Monster>("CORE_OVERLOAD_v" + to_string(wave), 200 + (wave * 20), 15 + wave, true);
        }
        else {
            enemy = make_unique<Monster>("Glitch_Drone_v" + to_string(wave), 40 + (wave * 10), 8 + wave);
        }

        while (hero->isAlive() && enemy->isAlive()) {
            hero->displayHUD();
            cout << "\n1. Attack | 2. Heal | 3. Special\nChoice: ";
            int choice; cin >> choice;

            if (choice == 1) hero->attack(*enemy);
            else if (choice == 2 && !hero->inventory.empty()) {
                hero->heal(hero->inventory.back().healAmount);
                hero->inventory.pop_back();
            }
            else if (choice == 3) hero->specialAbility(*enemy);

            if (enemy->isAlive()) enemy->attack(*hero);
        }

        if (hero->isAlive()) {
            cout << "\033[1;32m[WAVE CLEAR]\033[0m\n";
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
            cout << "\033[1;31m[GAME OVER]\033[0m\n";
            break;
        }
    }
    return 0;
}