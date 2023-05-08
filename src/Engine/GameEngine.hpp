#pragma once
class GameEngine {
public:
	void init();
	//std::shared_ptr<Scene> currentScene();
	void run();
	void update();
	void quit();
	//void changeScene(scene); // change the current scene's string
	//Assets& getAssets();
	//Window& window();
	void sUserInput();
private:
	//std::map<std::string, std::shared_ptr<Scene>> m_scenes;
	//sf::RenderWindow m_window;
	//Assets m_assets;
	//std::string m_currentScene; // Use this string to look up my scene in my map
	bool m_running;
};