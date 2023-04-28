#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>

void printConfig(void)
{
	std::cout << std::filesystem::current_path() << "\n";

	std::string data;
	std::ifstream fin("../../include/assignment1/configs/config.txt");

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML WORKING");

	unsigned int windowWidth, windowHeight;

	while (fin >> data) {
		if (data == "Window")
		{
			fin >> windowWidth >> windowHeight;
			std::cout << "windowWidth : " << windowWidth << " windowHeight : " << windowHeight << "\n";
		}
		std::cout << data << "\n";
	}

	window.close();
}

int assignment1(void)
{
	return 0;
}