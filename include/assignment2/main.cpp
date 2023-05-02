#include <SFML/Graphics.hpp>
#include "Game.hpp"

int runAssignment2() {

	Vec2 v1(100, 100);
	Vec2 v2(300, 500);

	Vec2 v3 = v1 + v2;

	std::cout << v3.x << " " << v3.y << "\n";

	Game g("../../include/assignment2/configs/config.txt");
	g.run();

	return 1;
}