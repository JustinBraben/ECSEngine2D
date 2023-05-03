#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"
#include <random>

Game::Game(const std::string& configPath) {
	init(configPath);
}


void Game::init(const std::string& config)
{
	// TODO: read in config file here
	// use the premade PlayerConfig, EnemyConfig, BulletConfig variables
	std::ifstream fin(config);
	std::string line;

	while (std::getline(fin, line)) {
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "Player") {
			ss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR
				>> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR
				>> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT
				>> m_playerConfig.V >> m_playerConfig.S;
		}
		if (type == "Enemy") {
			ss >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.OR
				>> m_enemyConfig.OG >> m_enemyConfig.OT >> m_enemyConfig.VMIN
				>> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI
				>> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
		}
		if (type == "Bullet") {
			ss >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.FR
				>> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR
				>> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT
				>> m_bulletConfig.V >> m_bulletConfig.L >> m_bulletConfig.S;
		}
	}

	// set up default window parameters
	m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}


void Game::run() {
	// TODO: add pause functionality in here
	// some systems should function while paused (rendering)
	// some systems shouldn't (movement / input)

	while (m_running) {
		//m_entities.update();

		if (!m_paused) {
			//sEnemySpawner();
			//sMovement();
			//sCollision();
			//sUserInput();
		}

		sRender();

		// increment the current frame
		// may need to be moved when paused implemented
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::sMovement()
{
	// TODO: implement movement
}

void Game::sUserInput()
{
	// TODO: implement user input

	sf::Event evnt;
	while (m_window.pollEvent(evnt)) {
		
		// this event triggers when the window is closed
		if (evnt.type == sf::Event::Closed)
			m_running = false;

		if (evnt.type == sf::Event::KeyPressed) {
			switch (evnt.key.code) {
			case sf::Keyboard::W:
				std::cout << "W Key Pressed\n";
				// TODO: set player's input component "up" to true
				break;
			default: break;
			}
		}

		if (evnt.type == sf::Event::KeyReleased) {
			switch (evnt.key.code) {
			case sf::Keyboard::W:
				std::cout << "W Key Released\n";
				// TODO: set player's input component "up" to false
				break;
			default: break;
			}
		}

		if (evnt.type == sf::Event::MouseButtonPressed) {
			if (evnt.mouseButton.button == sf::Mouse::Left) {
				std::cout << "Left Mouse Button clicked at (" << evnt.mouseButton.x << "." << evnt.mouseButton.y << ")\n";
				// TODO: call spawnBullet here
			}
			if (evnt.mouseButton.button == sf::Mouse::Right) {
				std::cout << "Right Mouse Button clicked at (" << evnt.mouseButton.x << "." << evnt.mouseButton.y << ")\n";
				// TODO: call spawnSpecialWeapon here
			}
		}
	}
}

void Game::sLifespan()
{
	// TODO: implement life span
} 

void Game::sRender()
{
	// TODO: change the code beliw to draw ALL of the entities
	// - sample drawing of the player Entity we have created
	m_window.clear();

	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	// draw the entity's sf::circleShape
	m_window.draw(m_player->cShape->circle);



	m_window.display();
}

void Game::sEnemySpawner()
{
	// TODO: implement enemy spawner
}

void Game::sCollision()
{
	// TODO: implement collision
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
	// TODO: Finish adding all properties of the player with the correct values from the config

	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	// Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle of 0

	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;
	
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(1.0f, 1.0f), 0.0f);
	
	// Entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Since we want this Entity to be our player, set our Game's player variable to be this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
	m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("enemy");

	// Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle of 0
	std::random_device rd;
	std::mt19937 gen(rd());

	// Generate random position
	std::uniform_int_distribution<> distribX(0, 1279);
	std::uniform_int_distribution<> distribY(0, 719);
	float ex = static_cast<float>(distribX(gen));
	float ey = static_cast<float>(distribY(gen));

	// TODO: set enemies based on config file
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 0.0f);

	// Entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(16.0f, 3, sf::Color(0, 0, 255), sf::Color(255, 255, 255), 4.0f);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: spawn small enemies at the location of the input enemy e

	// when we create the smaller enemy, we have to read the values of the original enemy
	// - spawn a number of small enemies equal to the vertices. of the orignal enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points of the original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	// TODO: implement the spawning of a bullet which travels toward target
	// - bullet speed is given as a scalar speed
	// - you must set the velocity by using the formula in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}
