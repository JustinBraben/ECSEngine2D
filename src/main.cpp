#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <map>
#include "Engine/Vec2.hpp"
#include "Engine/Entity.hpp"
//#include "../assignment1/main.cpp"

int main(int argc, char *argv[]) 
{
    //runAssignment1();

    // Enemy, all enemyEntities | Bullet, all bulletEntities

    Vec2 v1(100, 200);
    Vec2 v2(50, 75);
    Vec2 v3(1000, 2000);

    std::cout << v1.x << " " << v1.y << "\n";

    return 0;
}