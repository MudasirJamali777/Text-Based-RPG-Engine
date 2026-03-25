#pragma once
#include <string>

struct Item {
    std::string itemName;
    int healAmount;
};

struct Weapon {
    std::string name;
    int damageBonus;
    std::string rarity; // Common, Rare, Legendary
};