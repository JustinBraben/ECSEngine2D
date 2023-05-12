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
	
	// TODO: add paths to level files for std::vec<string> levelPaths
	m_levelPaths.push_back("../../include/assignment3/configs/level1.txt");

	// TODO: add strings to std::vec<string> menuStrings
	// These will be strings displayed in your main menu
	
	m_menuStrings.push_back("MEGA MARIO");
	m_menuStrings.push_back("LEVEL 1");
	m_menuStrings.push_back("UP : W    DOWN : S	    PLAY : D    BACK : ESC");

	// TODO: Set m_menuText
	//auto it = std::find(m_menuStrings.begin(), m_menuStrings.end(), "MEGA MARIO");
	// Set m_menuText
	auto menuFont = m_game->getAssets().getFont("Arial");
	m_menuText.setFont(menuFont);
	m_menuText.setString("MEGA MARIO");
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setCharacterSize(32);
	m_menuText.setPosition(10, 10);  // Adjust the position as needed
}

void Scene_Menu::update()
{
	// TODO: implement update for Scene_Menu
}

void Scene_Menu::onEnd()
{
	// TODO: implement onEnd for Scene_Menu
}

void Scene_Menu::sDoAction(const Action& action)
{
	if (action.type() == "START") {
		if (action.name() == "UP") {
			if (m_selectedMenuIndex > 0)
				m_selectedMenuIndex--;
			else
				m_selectedMenuIndex = m_menuStrings.size() - 1;
		}
		else if (action.name() == "DOWN") {
			m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
		}
		else if (action.name() == "PLAY") {
			//m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
		}
		else if (action.type() == "QUIT") {
			onEnd();
		}
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

	// TODO: remove this code and just set the font in the constructor
	// Currently not working in the constructor so setting it here
	auto menuFont = m_game->getAssets().getFont("Arial");
	m_menuText.setFont(menuFont);
	m_menuText.setString("MEGA MARIO");
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setCharacterSize(32);
	m_menuText.setPosition(10, 10);  // Adjust the position as needed

	m_game->window().draw(m_menuText);

	m_game->window().display();
}
