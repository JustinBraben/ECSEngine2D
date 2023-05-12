#include "Scene_Menu.hpp"

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
