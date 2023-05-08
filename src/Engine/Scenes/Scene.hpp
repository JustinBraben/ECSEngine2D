#pragma once
class Scene {
public:
	void update() = 0;
	void sDoAction(action) = 0;
	void sRender() = 0;

	void simulate(int val);
	void doAction(action);
	void registerAction(action);
private:
	// GameEngine m_game;
	// EntityManager m_entities;
	int m_currentFrame;
	// std::map<int, std::string> m_actionMap;
	bool paused;
	bool hasEnded;
};