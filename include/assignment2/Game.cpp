#include "Game.hpp"
#include <random>
#include <cmath>

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
			ss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S
				>> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB
				>> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
				>> m_playerConfig.OT >> m_playerConfig.V;
		}
		if (type == "Enemy") {
			ss >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN
				>> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG
				>> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN
				>> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
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
	m_window.setFramerateLimit(144);

	spawnPlayer();
}


void Game::run() {
	// TODO: add pause functionality in here
	// some systems should function while paused (rendering)
	// some systems shouldn't (movement / input)

	while (m_running) {
		m_entities.update();

		if (!m_paused) {
			sLifespan();
			sEnemySpawner();
			sMovement();
			sCollision();
		}

		// TODO: user input outside pause for now
		// Will need to allow user input for pausing
		// but disallow movement
		sUserInput();

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

	for (auto& entity : m_entities.getEntities()) {
		if (entity->tag() == "player") {
			// Sample movement speed update
			if (entity->cInput->up) {
				entity->cTransform->pos.y -= entity->cTransform->velocity.y;
			}
			if (entity->cInput->down) {
				entity->cTransform->pos.y += entity->cTransform->velocity.y;
			}
			if (entity->cInput->left) {
				entity->cTransform->pos.x -= entity->cTransform->velocity.x;
			}
			if (entity->cInput->right) {
				entity->cTransform->pos.x += entity->cTransform->velocity.x;
			}
		}

		if (entity->tag() == "bullet") {
			entity->cTransform->pos.y += entity->cTransform->velocity.y;
			entity->cTransform->pos.x += entity->cTransform->velocity.x;
		}

		if (entity->tag() == "enemy") {
			entity->cTransform->pos.y += entity->cTransform->velocity.y;
			entity->cTransform->pos.x += entity->cTransform->velocity.x;
		}
	}
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
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				std::cout << "S Key Pressed\n";
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				std::cout << "A Key Pressed\n";
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				std::cout << "D Key Pressed\n";
				m_player->cInput->right = true;
				break;
			default: break;
			}
		}

		if (evnt.type == sf::Event::KeyReleased) {
			switch (evnt.key.code) {
			case sf::Keyboard::W:
				std::cout << "W Key Released\n";
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				std::cout << "S Key Released\n";
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				std::cout << "A Key Released\n";
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				std::cout << "D Key Released\n";
				m_player->cInput->right = false;
				break;
			case sf::Keyboard::P:
				m_paused ? setPaused(false) : setPaused(true);
				if (m_paused) {
					std::cout << "Game Paused is : " << m_paused << "\n";
				}
				else {
					std::cout << "Game Paused is : " << m_paused << "\n";
				}
			default: break;
			}
		}

		if (evnt.type == sf::Event::MouseButtonPressed) {
			if (evnt.mouseButton.button == sf::Mouse::Left) {
				std::cout << "Left Mouse Button clicked at (" << evnt.mouseButton.x << "." << evnt.mouseButton.y << ")\n";
				if (!m_paused)
					spawnBullet(m_player, Vec2(evnt.mouseButton.x, evnt.mouseButton.y));
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
	//
	// for all entities
	// if entity has no lifespan component, skip
	// if entity has >0 remaining lifespan, subtract 1
	// if it has lifespan and is alive
	//	scale its alpha channel properly
	// if it has lifespan and its time is up
	//	destory the entity
	for (auto& entity : m_entities.getEntities()) {
		if (entity->cLifeSpan == nullptr)
			continue;
		if (entity->cLifeSpan->remaining > 0)
			entity->cLifeSpan->remaining--;
		if (entity->cLifeSpan != nullptr && entity->cLifeSpan->remaining < 1)
			entity->destroy();
	}
} 

void Game::sRender()
{
	// TODO: change the code beliw to draw ALL of the entities
	// - sample drawing of the player Entity we have created
	m_window.clear();

	for (auto e : m_entities.getEntities()) {

		// set the position of the shape based on the entity's transform->pos
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		// set the rotation of the shape based on the entity's transform->angle
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		// draw the entity's sf::CircleShape
		m_window.draw(e->cShape->circle);
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	// TODO: implement enemy spawner
	// - (use m_currentFrame - m_lastEnemySpawnTime) to determine
	// how long it has been since the last enemy spawned

	if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI)
		spawnEnemy();
}

void Game::sCollision()
{
	// TODO: implement collision
	for (auto bullet : m_entities.getEntities("bullet")) {
		for (auto enemy : m_entities.getEntities("enemy")) {
			// determine if a bullet is within the distance of an enemy 
			// if it is, destroy the enemy and the 
			// then spawn the small enemies from that enemy
			auto distance = bullet->cTransform->pos.dist(enemy->cTransform->pos);
			if (distance < bullet->cShape->circle.getRadius() + enemy->cShape->circle.getRadius()) {
				std::cout << "Bullet collided! with enemy : " << enemy->id() << " \n";
				enemy->destroy();
				bullet->destroy();
			}
		}
	}
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
	// TODO: Ensure these are not spawned offscreen
	// - also don't let them spawn on the player
	std::uniform_int_distribution<> distribVertices(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
	std::uniform_int_distribution<> distribX(20, 1250);
	std::uniform_int_distribution<> distribY(20, 699);
	std::uniform_real_distribution<> distribVX(-1, 1);
	std::uniform_real_distribution<> distribVY(-1, 1);
	
	int vertices = distribVertices(gen);
	float ex = static_cast<float>(distribX(gen));
	float ey = static_cast<float>(distribY(gen));
	float evx = static_cast<float>(distribVX(gen));
	float evy = static_cast<float>(distribVY(gen));


	// TODO: set enemies based on config file
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(evx, evy), 0.0f);

	// Entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, 
		sf::Color(0, 0, 0),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
		m_enemyConfig.OT);

	entity->cCollision = std::make_shared<CCollision>(32.0);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
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

	auto bullet = m_entities.addEntity("bullet");

	float pi = 3.141593;
	auto direction = mousePos - entity->cTransform->pos;
	auto length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length != 0)
		direction /= length;

	float speed = 1.0f;

	auto bulletVelocity = direction * speed;

	std::cout << "Velocity x of bullet : " << bulletVelocity.x << " Velocity y of bullet : " << bulletVelocity.y << "\n";

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(bulletVelocity.x, bulletVelocity.y), 0);
	bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255,255, 255), sf::Color(255,0,0), 2);
	bullet->cLifeSpan = std::make_shared<CLifeSpan>(144);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}
