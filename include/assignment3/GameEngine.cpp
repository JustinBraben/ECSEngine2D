#include "GameEngine.hpp"
#include "GameEngine.hpp"
#include "GameEngine.hpp"
#include "Assets.hpp"
#include "Scene_Play.hpp"
#include "Scene_Menu.hpp"

#include <iostream>

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	//m_assets.loadFromFile(path);

	m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
	m_window.setFramerateLimit(60);

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

const Assets& GameEngine::getAssets() const
{
	return m_assets;
}

void GameEngine::run()
{
	while (isRunning()) {
		update();
	}
}

void GameEngine::sUserInput()
{
	sf::Event evnt;
	while (m_window.pollEvent(evnt)) {

		if (evnt.type == sf::Event::Closed) {
			quit();
		}

		if (evnt.type == sf::Event::KeyPressed) {
			if (evnt.type == sf::Keyboard::X) {
				std::cout << "screenshot saved to " << "test.png" << "\n";
				sf::Texture texture;
				texture.create(m_window.getSize().x, m_window.getSize().y);
				texture.update(m_window);

				if (texture.copyToImage().saveToFile("test.png")) {
					std::cout << "screenshot saved to " << "test.png" << "\n";
				}
			}
		}

		if (evnt.type == sf::Event::KeyPressed || evnt.type == sf::Event::KeyReleased) {

			// if the current scene does not have an action associated with this key, skip the event
			if (currentScene()->getActionMap().find(evnt.key.code) == currentScene()->getActionMap().end())
				continue;

			// determine start or end action by whether it was key press or release
			const std::string actionType = (evnt.type == sf::Event::KeyPressed) ? "START" : "END";

			// look up the action and send the action to the scene
			currentScene()->doAction(Action(currentScene()->getActionMap().at(evnt.key.code), actionType));
		}
	}
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (m_sceneMap[sceneName])
		m_currentScene = sceneName;
	else {
		m_sceneMap[sceneName] = scene;
		m_currentScene = sceneName;
	}
}