#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <map>
#include "../assignment3/GameEngine.hpp"

int main(void) 
{
    // Game g("../../include/assignment2/configs/config.txt");
    // g.run();

    GameEngine g("../../include/assignment3/configs/assets.txt");
    g.run();

    return 0;
}