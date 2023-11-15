#pragma once

#include "Action.hpp"
#include "EntityManager.hpp"

#include <memory>

class GameEngine;	// forward declaration

typedef std::map<int, std::string> ActionMap;

class Scene {

protected:

	GameEngine* m_game = nullptr;
	EntityManager m_entityManager;
	ActionMap m_actionMap;
	bool m_paused = false;
	bool m_hasEnded = false;
	size_t m_currentFrame = 0;

	void setPaused(bool paused);


public:

	virtual void onEnd() = 0;
	virtual void update() = 0;
	virtual void setDelayMove();
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	Scene();
	Scene(GameEngine* gameEngine);

	void simulate(const size_t frames);
	void registerAction(int inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	bool hasEnded() const;
	const ActionMap& getActionMap() const;
	void drawLine(const Vec2& p1, const Vec2& p2);
};