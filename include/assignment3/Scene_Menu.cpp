#include "Scene_Menu.hpp"
#include "Scene_Play.hpp"

#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"

#include <iostream>

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine) 
{
	// Constructor implementation
	// You may initialize any member variables or perform other necessary actions here
	
	m_title = "MEGA MARIO";
	m_menuStrings = { "Level 1", "Level 2" };
	init();
}

void Scene_Menu::init()
{
	// TODO: Register actions for Scene_Menu
	registerAction(sf::Keyboard::Escape, "QUIT");	// QUIT out of the game
	registerAction(sf::Keyboard::W, "UP");			// Go UP in menu
	registerAction(sf::Keyboard::S, "DOWN");		// Go DOWN in menu
	registerAction(sf::Keyboard::D, "PLAY");		// Toggle drawing (G)rid

	// TODO: add paths to level files for std::vec<string> levelPaths
	// Load level paths from files
	// Suppose you have level#.txt files in a "levels" directory
	for (size_t i = 0; i < m_menuStrings.size(); ++i)
	{
		m_levelPaths.push_back("../../include/assignment3/configs/level" + std::to_string(i + 1) + ".txt");
	}

	// TODO: Set m_menuText
	//auto it = std::find(m_menuStrings.begin(), m_menuStrings.end(), "MEGA MARIO");
	// Set m_menuText
	auto& menuFont = m_game->getAssets().getFont("Arial");
	m_menuText.setFont(menuFont);
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setCharacterSize(32);
	m_menuText.setPosition(10, 10);  // Adjust the position as needed
}

void Scene_Menu::update()
{
	// TODO: implement update for Scene_Menu
	sRender();
}

void Scene_Menu::onEnd()
{
	// TODO: implement onEnd for Scene_Menu
}

void Scene_Menu::sDoAction(const Action& action)
{
	if (action.name() == "UP") {
		// Only decrease the index if it's not already at the top
		if (m_selectedMenuIndex > 0)
			m_selectedMenuIndex--;
		/*else
			m_selectedMenuIndex = m_menuStrings.size() - 1;*/
	}
	else if (action.name() == "DOWN") {
		// Only increase the index if it's not already at the bottom
		if (m_selectedMenuIndex < m_menuStrings.size() - 1)
			m_selectedMenuIndex++;
	}
	else if (action.name() == "PLAY") {
		auto newScenePlay = std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]);
		m_game->changeScene("PLAY", newScenePlay);
	}
	else if (action.type() == "QUIT") {
		onEnd();
	}
}

void Scene_Menu::sRender()
{
	// TODO: implement sRender for Scene_Menu
	//	This should display MEGA MARIO in the top left (Black text)
	//	and the level.txt names (Currently selected level will be white text,
	//	otherwise level names will be Black)
	//	Should display controls to navigate menu at the bottom of the screen
	//	eg UP: W	DOWN: S		PLAY: D		BACK: ESC	
	//

	m_game->window().clear(sf::Color(173, 216, 230));

	// Render title
	m_game->window().draw(m_menuText);

	// Render level names
	sf::Text levelText;
	auto& menuFont = m_game->getAssets().getFont("Arial");
	levelText.setFont(menuFont);
	levelText.setCharacterSize(24);
	for (size_t i = 0; i < m_menuStrings.size(); ++i)
	{
		levelText.setString(m_menuStrings[i]);
		// The currently selected level text will be white, the others will be black
		levelText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color::Black);
		// Position the level names below the title, adjust the position as needed
		levelText.setPosition(10, 50 + i * 30);
		m_game->window().draw(levelText);
	}

	// Render navigation instructions
	sf::Text instructions;
	instructions.setFont(menuFont);
	instructions.setCharacterSize(20);
	instructions.setFillColor(sf::Color::Black);
	instructions.setString("UP : W    DOWN : S    PLAY : D    BACK : ESC");
	instructions.setPosition(0, m_game->window().getSize().y - 50); // Bottom of the window
	m_game->window().draw(instructions);

	m_game->window().display();
}
