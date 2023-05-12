#include "Scene_Menu.hpp"

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
			//m_game->changeScene("Play", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
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

	//auto assets = m_game->getAssets();
	//auto font = assets.getFont("arial");
	auto fontMenu = m_game->getAssets().getFont("arial");

	sf::Text text;
	text.setFont(fontMenu);
	text.setString("MEGA MARIO");
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::Black);
	text.setPosition(10, 10);  // Adjust the position as needed

	m_game->window().draw(text);

	m_game->window().display();
}
