#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <map>
#include "../assignment2/Game.hpp"

int main(void) 
{
    //runAssignment1();
    // Enemy, all enemyEntities | Bullet, all bulletEntities
    //Vec2 v1(100, 200);
    //Vec2 v2(50, 75);
    //Vec2 v3(1000, 2000);
    //std::cout << v1.x << " " << v1.y << "\n";

    std::cout << "wtf\n";
    
    Game g("../../include/assignment2/configs/config.txt");
    g.run();

    return 0;
}