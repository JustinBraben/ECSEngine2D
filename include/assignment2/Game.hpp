#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Vec2.cpp"
class Game
{
public:
	Game(const std::string& configPath)
		: m_configPath(configPath) {}
	//~Game();

	void run() {

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

private:
	std::string m_configPath;
};