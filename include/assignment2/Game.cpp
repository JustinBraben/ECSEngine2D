#include "Game.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Game::Game(const std::string& configPath)
	: m_configPath(configPath) {}

void Game::run() {
	std::ifstream fin(m_configPath);
	std::string line;

	while (std::getline(fin, line)) {
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		std::string parameters;
		std::getline(ss, parameters);

		std::cout << type << parameters << "\n";
	}
}