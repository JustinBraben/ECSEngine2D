#pragma once

#include "Scene.hpp"
#include <map>
#include <memory>

#include "EntityManager.hpp"

class Scene_Play : public Scene {

	struct PlayerConfig {
		float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string WEAPON;
	};

protected:

	std::shared_ptr<Entity> m_player;
	std::string m_levelPath;
	PlayerConfig m_playerConfig;
	bool m_drawTextures = true;
	bool m_drawCollision = false;
	bool m_drawGrid = false;
	const Vec2 m_gridSize = { 64, 64 };
	sf::Text m_gridText;

	void init(const std::string& levelPath);

	void loadLevel(const std::string& filename);

public:
	void update();

	// Systems
	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sCollision();
	void sRender();
	//void sDoAction(action);
	void sDebug();
};