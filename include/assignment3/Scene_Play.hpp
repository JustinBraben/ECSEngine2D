#pragma once
#include "Scene.hpp"
#include "Entity.hpp"
#include <string>
#include <memory>
class Scene_Play : public Scene {
public:
	void init();
	void update();

	// Systems
	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sCollision();
	void sRender();
	//void sDoAction(action);
	void sDebug();
private:
	std::string m_levelPath;
	std::shared_ptr<Entity> m_player;
	//PlayerConfig m_playerConfig;
};