#pragma once

class Scene {
public:
	void virtual update() = 0;
	//void virtual sDoAction(action) = 0;
	void virtual sRender() = 0;

	void simulate(int val);
	//void doAction(action);
	//void registerAction(action);
private:
	// GameEngine m_game;
	// EntityManager m_entities;
	int m_currentFrame;
	// std::map<int, std::string> m_actionMap;
	bool paused;
	bool hasEnded;
};