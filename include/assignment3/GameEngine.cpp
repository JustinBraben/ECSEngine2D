#include "GameEngine.hpp"
#include "Assets.hpp"
#include "Scene_Play.hpp"
#include "Scene_Menu.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	std::ifstream fin(path);
	std::string line;

	// TODO: read the assets and add them to the m_assets class
	//	from assets.txt
	while (std::getline(fin, line)) {
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "Texture") {
			std::string textureName, texturePath;
			ss >> textureName >> texturePath;
			m_assets.addTexture(textureName, texturePath);
		}
		else if (type == "Animation") {
			std::string animationName, textureName;
			int frameCount, animSpeed, textureRectLeft, textureRectTop, textureSize;
			ss >> animationName >> textureName >> frameCount >> animSpeed >> textureRectLeft >> textureRectTop >> textureSize;

			Vec2 textureSizeVec = { static_cast<float>(textureSize), static_cast<float>(textureSize) };
			
			if (animSpeed == 0) 
			{
				const auto& tileRect = sf::IntRect(textureRectLeft, textureRectTop, textureSize, textureSize);
				m_assets.addAnimation(animationName, Animation(animationName, m_assets.getTexture(textureName), tileRect));
			}
			else {
				const auto& tileRect = sf::IntRect(textureRectLeft, textureRectTop, textureSize, textureSize);
				m_assets.addAnimation(animationName, Animation(animationName, m_assets.getTexture(textureName), tileRect, frameCount, animSpeed, textureSizeVec));
			}
		}
		else if (type == "Font") {
			std::string fontName, fontPath;
			ss >> fontName >> fontPath;
			m_assets.addFont(fontName, fontPath);
		}
	}

	m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
	m_window.setFramerateLimit(144);

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

void GameEngine::update()
{
	// TODO: update enitites and run systems
	//	that are meant to be run while the

	//auto scene = currentScene();

	sUserInput();
	
	// TODO: Should be calling update on the current scene
	// Then letting the scene dictate what systems need to be run
	currentScene()->update();
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

void GameEngine::quit()
{
	// TODO: Make quit() function
	// This should close the window if ESC action is sent to Scene_Menu
	// If it is called in a Scene_Play it should change scene back to the Scene_Menu
	// NOTE: for now it is just closing the window
	if (m_currentScene == "MENU") {
		m_running = false;
	}
}

void GameEngine::run()
{
	// TODO: add pause functionality in here
	// some systems should while pausing (rendering)
	// some systems shouldn't (movement / input)

	std::cout << "Game is now running\n";

	while (isRunning()) {
		update();
	}

	m_window.close();

	std::cout << "Game is now done running, Press Enter to continue...\n";

	//std::cin.ignore();  // Ignore any characters in the input buffer
	std::cin.get();     // Wait for the user to press Enter
}

void GameEngine::sUserInput()
{
	sf::Event evnt;
	while (m_window.pollEvent(evnt)) {

		if (evnt.type == sf::Event::Closed) {
			quit();
		}

		if (evnt.type == sf::Event::KeyPressed) {
			if (evnt.key.code == sf::Keyboard::X) {
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

			auto scene = currentScene();

			// if the current scene does not have an action associated with this key, skip the event
			if (currentScene()->getActionMap().find(evnt.key.code) == currentScene()->getActionMap().end())
				continue;

			// determine start or end action by whether it was key press or release
			const std::string actionType = (evnt.type == sf::Event::KeyPressed) ? "START" : "END";

			// look up the action and send the action to the scene
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(evnt.key.code), actionType));
		}
		
		if (evnt.type == sf::Event::MouseButtonPressed || evnt.type == sf::Event::MouseButtonReleased) {

			auto scene = currentScene();

			// if the current scene does not have an action associated with this key, skip the event
			if (currentScene()->getActionMap().find(evnt.mouseButton.button) == currentScene()->getActionMap().end())
				continue;

			if (evnt.mouseButton.button == sf::Mouse::Left)
				continue;

			// determine start or end action by whether it was key press or release
			const std::string actionType = (evnt.type == sf::Event::MouseButtonPressed) ? "START" : "END";

			// look up the action and send the action to the scene
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(evnt.mouseButton.button), actionType));
		}
	}
	
	// Check if the window is closed after processing events
	/*if (!isRunning()) {
		m_window.close();
	}*/
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	// TODO: implement changeScene for the gameEngine
	//	this should check to see if the sceneName is contained in the m_sceneMap
	//	if the sceneName is contained in the m_sceneMap, set the m_currentScene string to the sceneName
	//	otherwise, it should create a new entry in the m_sceneMap with sceneName as key and scene as 
	//	std::shared_ptr<Scene> value
	//	FIND OUT WHAT endCurrentScene IS FOR

	// Check if the scene already exists in the map
	auto it = m_sceneMap.find(sceneName);

	if (it != m_sceneMap.end()) {
		// Scene already exists, set it as the current scene
		m_currentScene = sceneName;

		float windowCenterX = (this)->window().getSize().x / 2.0f;
		float windowCenterY = (this)->window().getSize().y / 2.0f;
		sf::View view = (this)->window().getView();
		view.setCenter(windowCenterX, windowCenterY);

		(this)->window().setView(view);

		// Optionally end the current scene if requested
		if (endCurrentScene)
		{
			// Call any necessary cleanup or end functions on the current scene
			// it->second->end();
		}
	}
	else {
		// Add the new scene to the map
		m_sceneMap[sceneName] = scene;
		m_currentScene = sceneName;
		if (m_currentScene == "MENU") {
			// If its menu set delay to true 
			m_sceneMap[sceneName]->setDelayMove();
		}
	}
}