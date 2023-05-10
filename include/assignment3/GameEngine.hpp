#pragma once

#include "Scene.hpp"
#include "Assets.hpp"

#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine {

protected:
	sf::RenderWindow m_window;
	Assets m_assets;
	SceneMap m_currentScene;
	size_t m_simulationSpeed = 1;
	bool m_running = true;

	void init();
	void update();

	void sUserInput();

	std::shared_ptr<Scene> currentScene();

public:

	GameEngine(const std::string& path);

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false); // change the current scene's string

	void quit();
	void run();

	sf::RenderWindow& window();
	const Assets& getAssets() const;
	bool isRunning();
};