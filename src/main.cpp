#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <map>
#include "../assignment2/Game.hpp"

int main(void) 
{
    Game g("../../include/assignment2/configs/config.txt");
    g.run();

    return 0;
}