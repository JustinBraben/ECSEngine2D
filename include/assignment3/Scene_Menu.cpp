#include "Scene_Menu.hpp"

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
}
