#include "Game.hpp"
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
			ss >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S
				>> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
				>> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB
				>> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
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

	m_window.close();
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
			if (entity->cInput->up && entity->cTransform->pos.y > entity->cCollision->radius) {
				entity->cTransform->pos.y -= entity->cTransform->velocity.y;
			}
			if (entity->cInput->down && entity->cTransform->pos.y < m_window.getSize().y - entity->cCollision->radius) {
				entity->cTransform->pos.y += entity->cTransform->velocity.y;
			}
			if (entity->cInput->left && entity->cTransform->pos.x > entity->cCollision->radius) {
				entity->cTransform->pos.x -= entity->cTransform->velocity.x;
			}
			if (entity->cInput->right && entity->cTransform->pos.x < m_window.getSize().x - entity->cCollision->radius) {
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
		
		if (entity->tag() == "smallEnemy") {
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
			case sf::Keyboard::Escape:
				std::cout << "Escape Key Pressed\n";
				m_running = false;
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
		if (entity->cLifeSpan->remaining > 0) {
			entity->cLifeSpan->remaining--;
			auto entColor = entity->cShape->getFillColor();
			auto alphaDelta = static_cast<float>(255) / static_cast<float>(entity->cLifeSpan->total);
			auto newAlpha = alphaDelta * static_cast<float>(entity->cLifeSpan->remaining);
			auto newColor = sf::Color(entColor.r, entColor.g, entColor.b, newAlpha);
			entity->cShape->setFillColor(newColor);
			entity->cShape->setOutlineColor(newColor);
		}
		if (entity->cLifeSpan != nullptr && entity->cLifeSpan->remaining < 1)
			entity->destroy();
	}
} 

void Game::sRender()
{
	// TODO: change the code beliw to draw ALL of the entities
	// - sample drawing of the player Entity we have created
	m_window.clear();

	for (auto entity : m_entities.getEntities()) {

		// set the position of the shape based on the entity's transform->pos
		entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);

		// set the rotation of the shape based on the entity's transform->angle
		entity->cTransform->angle += 1.0f;
		entity->cShape->circle.setRotation(entity->cTransform->angle);

		// TODO: adjust alpha of entities that have a lifespan
		// decrease alpha as life span gets closer to 0 from total
		if (entity->cLifeSpan != nullptr) {
			//entity->cShape->circle.setFillColor();
		}

		// draw the entity's sf::CircleShape
		m_window.draw(entity->cShape->circle);
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
			if (distance < bullet->cCollision->radius + enemy->cCollision->radius) {
				std::cout << "Bullet collided! with enemy : " << enemy->id() << " \n";
				spawnSmallEnemies(enemy);
				enemy->destroy();
				bullet->destroy();
			}
		}
	}

	// TODO: write the collisions where enemy runs into the player
	for (auto enemy : m_entities.getEntities("enemy")) {

		// TODO: make enemies reverse velocity when they collide with a bound

		if (enemy->cTransform->pos.x < enemy->cCollision->radius ||
			enemy->cTransform->pos.x > m_window.getSize().x - enemy->cCollision->radius) {
			enemy->cTransform->velocity.x *= -1.0f;
		}
		if (enemy->cTransform->pos.y < enemy->cCollision->radius ||
			enemy->cTransform->pos.y > m_window.getSize().y - enemy->cCollision->radius) {
			enemy->cTransform->velocity.y *= -1.0f;
		}

		// Check if out of bounds

		// TODO: respawn player when colliding with an enemy
		auto distance = m_player->cTransform->pos.dist(enemy->cTransform->pos);
		if (distance < m_player->cCollision->radius + enemy->cCollision->radius) {
			std::cout << "Player collided with enemy id : " << enemy->id() << "\n";
			m_player->cTransform->pos.x = m_window.getSize().x / 2;
			m_player->cTransform->pos.y = m_window.getSize().y / 2;
			spawnSmallEnemies(enemy);
			enemy->destroy();
		}
	}

	// TODO: write collisions with small Enemies
	for (auto enemy : m_entities.getEntities("smallEnemy")) {
		auto distance = m_player->cTransform->pos.dist(enemy->cTransform->pos);
		if (distance < m_player->cCollision->radius + enemy->cCollision->radius) {
			std::cout << "Player collided with enemy id : " << enemy->id() << "\n";
			m_player->cTransform->pos.x = m_window.getSize().x / 2;
			m_player->cTransform->pos.y = m_window.getSize().y / 2;
			enemy->destroy();
		}
	}
}

void Game::sScore()
{

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
	
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), 
		Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
	
	// Entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V,
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Add collision to the player 
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	entity->cScore = std::make_shared<CScore>(0);

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

	std::random_device rd;
	std::mt19937 gen(rd());

	// Generate random position
	// TODO: Ensure these are not spawned offscreen
	// - also don't let them spawn on the player
	std::uniform_int_distribution<> distribVertices(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
	std::uniform_int_distribution<> distribX(m_enemyConfig.SR, m_window.getSize().x - m_enemyConfig.SR);
	std::uniform_int_distribution<> distribY(m_enemyConfig.SR, m_window.getSize().y - m_enemyConfig.SR);
	std::uniform_int_distribution<> distribRGB(0, 255);

	std::uniform_real_distribution<> distribVX(-m_enemyConfig.SMIN, m_enemyConfig.SMAX);
	std::uniform_real_distribution<> distribVY(-m_enemyConfig.SMIN, m_enemyConfig.SMAX);

	
	int vertices = distribVertices(gen);
	float ex = static_cast<float>(distribX(gen));
	float ey = static_cast<float>(distribY(gen));
	float evx = distribVX(gen);
	float evy = distribVY(gen);

	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	// TODO: set enemies based on config file
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(evx, evy), 0.0f);

	while (entity->cTransform->pos.dist(m_player->cTransform->pos) <= entity->cCollision->radius + m_player->cCollision->radius) {

		float ex = static_cast<float>(distribX(gen));
		float ey = static_cast<float>(distribY(gen));
		entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(evx, evy), 0.0f);

	}

	// Entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, 
		sf::Color(distribRGB(gen), distribRGB(gen), distribRGB(gen)),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
		m_enemyConfig.OT);

	entity->cScore = std::make_shared<CScore>(vertices * 10);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: spawn small enemies at the location of the input enemy e
	std::cout << "This would have made " << entity->cShape->circle.getPointCount() << " many small enemies\n";

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> distribV(m_enemyConfig.SMIN, m_enemyConfig.SMAX);

	float speed = distribV(gen);

	for (int i = 0; i < entity->cShape->circle.getPointCount(); i++) {
		float pi = 3.141593;
		auto smallerEntity = m_entities.addEntity("smallEnemy");
		auto angleToRadians = (pi / 180.0f);
		auto baseAngle = 360.0f / static_cast<float>(entity->cShape->circle.getPointCount());

		//auto angle = i * (2.0f * pi) / static_cast<float>(entity->cShape->circle.getPointCount()) * angleToRadians;
		auto angle = i * (baseAngle) * angleToRadians;

		std::cout << "angle would have been " << angle << "\n";

		float evx = speed * std::cos(angle);
		float evy = speed * std::sin(angle);

		smallerEntity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR - m_enemyConfig.CR / 2);

		smallerEntity->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y),
			Vec2(evx, evy), 0);

		smallerEntity->cShape = std::make_shared<CShape>(static_cast<float>(m_enemyConfig.SR) / 2.0f,
			entity->cShape->circle.getPointCount(),
			entity->cShape->circle.getFillColor(), entity->cShape->circle.getOutlineColor(),
			m_enemyConfig.OT);

		smallerEntity->cLifeSpan = std::make_shared<CLifeSpan>(m_enemyConfig.L);

		smallerEntity->cScore = std::make_shared<CScore>(entity->cScore->score * 10);
	}

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

	auto bulletVelocity = direction * m_bulletConfig.S;

	std::cout << "Velocity x of bullet : " << bulletVelocity.x << " Velocity y of bullet : " << bulletVelocity.y << "\n";

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(bulletVelocity.x, bulletVelocity.y), 0);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, 
		sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
		m_bulletConfig.OT);
	bullet->cLifeSpan = std::make_shared<CLifeSpan>(m_bulletConfig.L);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}
