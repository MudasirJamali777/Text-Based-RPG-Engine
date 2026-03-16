#include <iostream>
#include <memory>
#include "Character.h"

int main() {
    // Making the terminal look cool with colors
    std::cout << "\033[1;31m" << "=== SHADOW REALM ENGINE v1.0 ===" << "\033[0m" << std::endl;

    // Creating a character using a Smart Pointer (Best Practice!)
    auto player = std::make_unique<Character>("MK_Slayer", 100, 20);

    std::cout << "Character Created: " << player->getName() << std::endl;

    // Simulating a quick hit
    player->takeDamage(15);

    std::cout << "\n\033[1;30m" << "Press Enter to exit..." << "\033[0m";
    std::cin.get();
    return 0;
}