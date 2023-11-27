#pragma once

#include "Scene.hpp"
#include <map>
#include <memory>
#include <random>

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
	std::vector<std::string> m_aiBehaviours;

	void init(const std::string& levelPath);

	void loadLevel(const std::string& filename);

private:
	sf::Time m_scenePlayTime;   // Delay time between moves
	sf::Clock m_scenePlayClock; // Clock to track the delay time
	std::random_device m_randomDevice;
	std::mt19937 m_randomGenerator;
public:

	Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

	Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

	Vec2 pixelToGrid(float pixelX, float pixelY);

	void spawnPlayer(float posX, float posY);
	std::shared_ptr<Entity> spawnEnemy(float posX, float posY, std::string& aiType);
	void spawnBullet(std::shared_ptr<Entity> entity);
	bool canCollide(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);
	void setTile();
	void removeTile();
	void playerMovement();
	void enemyMovement();

	void update() override;
	void onEnd() override;
	void sDoAction(const Action& action) override;
	void sRender() override;

	void drawLine(const Vec2& p1, const Vec2& p2);

	size_t width() const;

	size_t height() const;

	// Systems
	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sLifespan();
	void sCollision();
	void sDebug();
};