#include "../Scene_Play.hpp"
#include "../Scene_Menu.hpp"
#include "../Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ranges>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine), m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");		// Toggle drawing (T)extures
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");	// Toggle drawing (C)ollision Boxes
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");			// Toggle drawing (G)rid
	registerAction(sf::Mouse::Right, "SET_TILE");			// Set tiles to add to current scene and associated level txt
	registerAction(sf::Mouse::Button::XButton2, "REMOVE_TILE");		// Remove tiles from current scene and associated level txt

	//registerAction(sf::Keyboard::W, "JUMP");

	// TODO: Register all other gameplay Actions
	registerAction(sf::Keyboard::W, "MOVE_UP");
	registerAction(sf::Keyboard::S, "MOVE_DOWN");
	registerAction(sf::Keyboard::A, "MOVE_LEFT");
	registerAction(sf::Keyboard::D, "MOVE_RIGHT");

	registerAction(sf::Keyboard::Space, "SHOOT");

	m_gridText.setCharacterSize(12);
	//m_gridText.setFont(m_game->getAssets().getFont("Tech"));

	auto& gridFont = m_game->getAssets().getFont("Arial");
	m_gridText.setFont(gridFont);
	m_gridText.setFillColor(sf::Color::Black);
	m_gridText.setCharacterSize(12);

	loadLevel(levelPath);

	m_randomGenerator = std::mt19937(m_randomDevice());
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	// TODO: this function takes in a grid (x,y) and an Entity
	//	Return a Vec2 indiciating where the CENTER position of the Entity should be
	//	You must use the Entity's Animation size to position it correctly
	//	The size of the grid width aand height is stored in m_gridSize.x and m_gridSize.y
	//	The bottom-left corner of the Animation should align with the bottom left of the grid cell

	auto entityAnimation = entity->getComponent<CAnimation>();

	// Setting this to transform size x at the moment
	// TODO: Should change this if you want to adjust the x and y scale
	auto entitySize = entityAnimation.animation.getSize() * std::abs(entity->getComponent<CTransform>().scale.x);
	auto posX = gridX * entitySize.x;
	auto posY = m_game->window().getSize().y - (gridY * entitySize.y);
	auto midX = posX + (entitySize.x / 2.0f);
	auto midY = posY - (entitySize.y / 2.0f);

	return Vec2(midX, midY);
}

Vec2 Scene_Play::pixelToGrid(float pixelX, float pixelY)
{
	auto tilePositionX = std::floor(pixelX / m_gridSize.x);
	auto tilePositionY = std::trunc((pixelY - height()) / m_gridSize.y) * -1;
	return Vec2(tilePositionX, tilePositionY);
}

void Scene_Play::loadLevel(const std::string& filename)
{
	// reset the entity manager every time we load a level
	m_entityManager = EntityManager();

	// TODO: read in the level file and add the appropriate entities
	//	use the PlayerConfig struct m_playerConfig to store player properties
	//	this struct is defined at the top of Scene_Play.hpp
	std::ifstream fin(filename);
	std::string line;

	while (std::getline(fin, line)) {
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "Tile") {
			std::string tileType;
			float gridX, gridY;
			ss >> tileType >> gridX >> gridY;

			auto brick = m_entityManager.addEntity(type);
			auto& assets = m_game->getAssets();
			auto& animation = m_game->getAssets().getAnimation(tileType);
			brick->getComponent<CTransform>().scale.x = 64.0f / animation.getSize().x;
			brick->getComponent<CTransform>().scale.y = 64.0f / animation.getSize().y;
			brick->addComponent<CAnimation>(animation, true);
			brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));
			brick->addComponent<CBoundingBox>(Vec2(animation.getSize().x * brick->getComponent<CTransform>().scale.x, 
				animation.getSize().y * brick->getComponent<CTransform>().scale.y));
		}
		if (type == "Dec")
		{
			std::string tileType;
			float gridX, gridY;
			ss >> tileType >> gridX >> gridY;

			auto dec = m_entityManager.addEntity(type);
			auto& assets = m_game->getAssets();
			auto& animation = m_game->getAssets().getAnimation(tileType);
			dec->getComponent<CTransform>().scale.x = 64.0f / animation.getSize().x;
			dec->getComponent<CTransform>().scale.y = 64.0f / animation.getSize().y;
			dec->addComponent<CAnimation>(animation, true);
			dec->addComponent<CTransform>(gridToMidPixel(gridX, gridY, dec));
		}

		if (type == "Hazard")
		{
			std::string tileType;
			float gridX, gridY;
			ss >> tileType >> gridX >> gridY;

			auto hazard = m_entityManager.addEntity(type);
			auto& assets = m_game->getAssets();
			auto& animation = m_game->getAssets().getAnimation(tileType);
			hazard->getComponent<CTransform>().scale.x = 64.0f / animation.getSize().x;
			hazard->getComponent<CTransform>().scale.y = 64.0f / animation.getSize().y;
			hazard->addComponent<CAnimation>(animation, true);
			hazard->addComponent<CTransform>(gridToMidPixel(gridX, gridY, hazard));

			hazard->addComponent<CBoundingBox>(Vec2(animation.getSize().x * hazard->getComponent<CTransform>().scale.x,
				animation.getSize().y * hazard->getComponent<CTransform>().scale.y));

			if (tileType == "SpikeTrap") 
			{
				//hazard->getComponent<CBoundingBox>().size.y /= 2;
				//hazard->getComponent<CBoundingBox>().halfSize.y = hazard->getComponent<CBoundingBox>().size.y / 2;
				hazard->getComponent<CTransform>().pos.y += hazard->getComponent<CBoundingBox>().halfSize.y * 1.5f;
			}
		}
	}

	// NOTE: all of the code below is sample code which shows you hot to
	//	set up and use entities with the new syntax, it should be removed
	std::string aiBackandForth = "BackAndForth";
	std::string aiJumping = "Jumping";

	spawnPlayer(16, 5);
	spawnEnemy(3, 2, aiBackandForth);
	spawnEnemy(11, 7, aiJumping);
	spawnEnemy(43, 3, aiBackandForth);
	spawnEnemy(80, 2, aiBackandForth);

	// some sample entities
	// IMPORTANT: always add the CAnimation component first so that gridToMidPixel can compute correctly
	//brick->addComponent<CAnimation>(m_game->assets().getAnimation("Brick"), true);

	// NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
	//	Components are now returned as references rather than pointers
	//	If you do not specify a reference variable type, it will COPY the component
	//	Here is an example:

	//	This will COPY the transform into the variable 'transform1' - it is INCORRECT
	//	Any changes you make to transform1 will not be changed inside the entity
	//	auto tranform1 = entity->get<CTransform>();

	//	This will REFERENCE (&) the transform with the variable 'transform2' it is CORRECT
	//	Now any changes you make to transform2 will be changed inside the entity
	//	auto& transform2 = entity->get<CTransform>();
}

void Scene_Play::spawnPlayer(float posX, float posY)
{
	// here is a sample player entity which you can use to construct other entities
	m_player = m_entityManager.addEntity("Player");
	m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("PlayerIdle"), true);
	m_player->getComponent<CTransform>().scale.x = 64.0f / m_player->getComponent<CAnimation>().animation.getSize().x;
	m_player->getComponent<CTransform>().scale.y = 64.0f / m_player->getComponent<CAnimation>().animation.getSize().x;
	m_player->addComponent<CTransform>(gridToMidPixel(posX, posY, m_player));
	m_player->addComponent<CGravity>(0.1f);
	m_player->addComponent<CBoundingBox>(Vec2(64.0f, 64.0f));
	m_player->addComponent<CState>("AIR");

	// TODO: be sure to add the remaining components to the player
}

std::shared_ptr<Entity> Scene_Play::spawnEnemy(float posX, float posY, std::string& aiType)
{
	auto enemy = m_entityManager.addEntity("Enemy");
	enemy->addComponent<CAnimation>(m_game->getAssets().getAnimation("EnemyIdle"), true);
	enemy->getComponent<CTransform>().scale.x = 64.0f / enemy->getComponent<CAnimation>().animation.getSize().x;
	enemy->getComponent<CTransform>().scale.y = 64.0f / enemy->getComponent<CAnimation>().animation.getSize().x;
	enemy->addComponent<CTransform>(gridToMidPixel(posX, posY, enemy));
	enemy->addComponent<CGravity>(0.1f);
	enemy->addComponent<CBoundingBox>(Vec2(64.0f, 64.0f));
	enemy->addComponent<CState>("AIR");
	enemy->addComponent<CAi>(aiType);
	if (enemy->getComponent<CAi>().behaviour == "Jumping")
	{
		enemy->addComponent<CInput>();
		enemy->getComponent<CInput>().up = true;
	}
	if (enemy->getComponent<CAi>().behaviour == "BackAndForth")
	{
		enemy->addComponent<CInput>();
		enemy->getComponent<CInput>().right = true;
	}
	return enemy;
}

void Scene_Play::sEnemySpawner()
{
	m_scenePlayTime = m_scenePlayClock.getElapsedTime();
	sf::Time t1 = sf::seconds(5);

	if (m_scenePlayTime > t1)
	{
		std::uniform_int_distribution<> distributionX(2, 51);
		std::uniform_int_distribution<> distributionY(3, 8);

		std::string jumping = "Jumping";

		bool successfulCreate = false;

		while (!successfulCreate)
		{
			int randomX = distributionX(m_randomGenerator);
			int randomY = distributionY(m_randomGenerator);

			auto enemy = spawnEnemy(randomX, randomY, jumping);

			// Check for collisions with existing entities
			bool collision = false;

			// Loop through entities and make sure there isnt any collisions with existing entities
			for (auto entity : m_entityManager.getEntities())
			{
				if (entity->tag() == "Enemy")
				{
					continue;
				}

				if (!canCollide(enemy, entity))
				{
					continue;
				}

				Physics physics;

				Vec2 overlap = physics.GetOverlap(entity, enemy);

				if (overlap.x > 0 && overlap.y)
				{
					collision = true;
					enemy->destroy();
					break;
				}
			}

			if (!collision)
			{
				successfulCreate = true;
				std::cout << "5 seconds have passed, an enemy will spawn at (" << randomX << ", " << randomY << ")\n";
			}
		}

		m_scenePlayClock.restart();
	}
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	// TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
	auto m_bullet = m_entityManager.addEntity("Bullet");
	m_bullet->addComponent<CAnimation>(m_game->getAssets().getAnimation("Bullet"), true);
	m_bullet->addComponent<CTransform>();
	m_bullet->addComponent<CBoundingBox>(Vec2(32.0f, 32.0f));

	// TODO: Find what direction the player is facing.
	// Spawn the bullet on the side the player is facing
	m_bullet->getComponent<CTransform>().pos.x = entity->getComponent<CTransform>().pos.x;
	m_bullet->getComponent<CTransform>().pos.y = entity->getComponent<CTransform>().pos.y;

	if (entity->getComponent<CTransform>().scale.x > 0)
		m_bullet->getComponent<CTransform>().velocity.x = 6.0f;
	else
		m_bullet->getComponent<CTransform>().velocity.x = -6.0f;

	// TODO: Set lifespan for bullet
	m_bullet->addComponent<CLifeSpan>(90, 0);
}

bool Scene_Play::canCollide(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2)
{
	bool canCollideX = std::abs(entity1->getComponent<CTransform>().pos.x - entity2->getComponent<CTransform>().pos.x) <
		entity1->getComponent<CBoundingBox>().halfSize.x + entity2->getComponent<CBoundingBox>().halfSize.x;

	bool canCollideY = std::abs(entity1->getComponent<CTransform>().pos.y - entity2->getComponent<CTransform>().pos.y) <
		entity1->getComponent<CBoundingBox>().halfSize.y + entity2->getComponent<CBoundingBox>().halfSize.y;

	return canCollideX && canCollideY;
}

void Scene_Play::update()
{
	m_entityManager.update();

	// TODO: implement pause functionality

	sEnemySpawner();
	sMovement();
	sLifespan();
	sCollision();
	sAnimation();
	sRender();
}

void Scene_Play::setTile()
{
	const auto mousePos = sf::Mouse::getPosition(m_game->window());
	const auto mouseWorldPos = m_game->window().mapPixelToCoords(mousePos);

	const auto gridPos = pixelToGrid(mouseWorldPos.x, mouseWorldPos.y);

	//std::cout << "Mouse in world position (" << mouseWorldPos.x << "," << mouseWorldPos.y << ")\n";
	//std::cout << "Mouse in tile position (" << gridPos.x << "," << gridPos.y << ")\n";

	//std::cout << "Level path is : " << m_levelPath << "\n;";

	// Specify the file path
	std::string filePath = m_levelPath;

	// Read the contents of the file into a vector of strings
	std::ifstream inputFile(filePath);
	std::vector<std::string> lines;
	std::string line;

	while (std::getline(inputFile, line))
	{
		lines.push_back(line);
	}

	// Check if the file has at least two lines
	if (lines.size() >= 2)
	{
		// Insert a line with a tile at the second last position
		std::string newTile = "Tile\t\t\tOryxBrick\t\t\t" + std::to_string(static_cast<int>(gridPos.x)) + "\t" + std::to_string(static_cast<int>(gridPos.y));

		// Check if the line already exists in the file
		if (std::find(lines.begin(), lines.end(), newTile) == lines.end())
		{
			// Line does not exist, insert it
			lines.insert(lines.end() - 1, newTile);

			// Write the modified content back to the file
			std::ofstream outputFile(filePath);

			for (size_t i = 0; i < lines.size() - 1; ++i)
			{
				outputFile << lines[i] << "\n";
			}

			// Write the last line without adding an extra newline
			outputFile << lines.back();

			std::cout << newTile << "\n";
			std::cout << "New tile inserted at the second last position.\n";

			auto brick = m_entityManager.addEntity("Tile");
			auto& assets = m_game->getAssets();
			auto& animation = m_game->getAssets().getAnimation("OryxBrick");
			brick->getComponent<CTransform>().scale.x = 64.0f / animation.getSize().x;
			brick->getComponent<CTransform>().scale.y = 64.0f / animation.getSize().y;
			brick->addComponent<CAnimation>(animation, true);
			brick->addComponent<CTransform>(gridToMidPixel(gridPos.x, gridPos.y, brick));
			brick->addComponent<CBoundingBox>(Vec2(animation.getSize().x * brick->getComponent<CTransform>().scale.x,
				animation.getSize().y * brick->getComponent<CTransform>().scale.y));
		}
		else
		{
			std::cout << "Duplicate line found. Not inserting a new tile.\n";
		}
	}
	else
	{
		std::cerr << "The file does not have enough lines.\n";
	}
}

void Scene_Play::removeTile()
{
	const auto mousePos = sf::Mouse::getPosition(m_game->window());
	const auto mouseWorldPos = m_game->window().mapPixelToCoords(mousePos);

	const auto gridPos = pixelToGrid(mouseWorldPos.x, mouseWorldPos.y);

	//std::cout << "Mouse in world position (" << mouseWorldPos.x << "," << mouseWorldPos.y << ")\n";
	//std::cout << "Mouse in tile position (" << gridPos.x << "," << gridPos.y << ")\n";

	//std::cout << "Level path is : " << m_levelPath << "\n;";

	// Specify the file path
	std::string filePath = m_levelPath;

	// Specify the content to be removed
	std::string tileLineToRemove = "Tile\t\t\tOryxBrick\t\t\t" + std::to_string(static_cast<int>(gridPos.x)) + "\t" + std::to_string(static_cast<int>(gridPos.y));

	// Read the contents of the file into a vector of strings
	std::ifstream inputFile(filePath);
	std::vector<std::string> lines;
	std::string line;

	while (std::getline(inputFile, line))
	{
		// Check if the line contains the content to be removed
		if (line.find(tileLineToRemove) == std::string::npos)
		{
			// If not, add the line to the vector
			lines.push_back(line);
		}
	}

	// Write the modified content back to the file
	std::ofstream outputFile(filePath);

	for (const auto& line : lines) {
		outputFile << line << "\n";
	}

	std::cout << "Lines containing \"" << tileLineToRemove << "\" removed.\n";

	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->tag() == "Tile")
		{
			auto entityGridPos = pixelToGrid(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
			if (entityGridPos.x == gridPos.x && entityGridPos.y == gridPos.y)
				entity->destroy();
		}
	}
}

void Scene_Play::playerMovement()
{
	// TODO: Implement player movement / jumping based on its CInput component
	// TODO: Implement gravity's effect on the player
	// TODO: Implement the maximum player speed in both X and Y directions
	// NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right

	Vec2 playerVelocity(m_player->getComponent<CTransform>().velocity.x, m_player->getComponent<CTransform>().velocity.y);

	if (m_player->getComponent<CInput>().up)
	{
		playerVelocity.y += m_player->getComponent<CInput>().canJump ? -0.6f : 0;
	}
	if (m_player->getComponent<CInput>().down)
	{
		playerVelocity.y += 0.3f;
	}

	if (m_player->getComponent<CInput>().left)
	{
		playerVelocity.x += -0.2f;
		m_player->getComponent<CTransform>().scale.x = -1 * std::abs(m_player->getComponent<CTransform>().scale.x);
	}

	if (m_player->getComponent<CInput>().right)
	{
		playerVelocity.x += 0.2f;
		m_player->getComponent<CTransform>().scale.x = std::abs(m_player->getComponent<CTransform>().scale.x);
	}

	// If player is moving left but the left key is not being pressed, gradually slow them down
	if (!m_player->getComponent<CInput>().left && playerVelocity.x < 0)
	{
		playerVelocity.x += 0.2f;

		if (playerVelocity.x > 0)
			playerVelocity.x = 0;
	}

	// If player is moving right but the right key is not being pressed, gradually slow them down
	if (!m_player->getComponent<CInput>().right && playerVelocity.x > 0)
	{
		playerVelocity.x -= 0.2f;

		if (playerVelocity.x < 0)
			playerVelocity.x = 0;
	}

	m_player->getComponent<CTransform>().velocity = playerVelocity;

	// Clamp player velocity in either direction
	if (m_player->getComponent<CTransform>().velocity.y > 6.0f)
		m_player->getComponent<CTransform>().velocity.y = 6.0f;

	if (m_player->getComponent<CTransform>().velocity.x > 4.0f)
		m_player->getComponent<CTransform>().velocity.x = 4.0f;

	if (m_player->getComponent<CTransform>().velocity.y < -6.0f) {
		m_player->getComponent<CTransform>().velocity.y = -6.0f;
		m_player->getComponent<CInput>().canJump = false;
	}

	if (m_player->getComponent<CTransform>().velocity.x < -4.0f)
		m_player->getComponent<CTransform>().velocity.x = -4.0f;

	// If player is moving upwards they are in the air
	if (m_player->getComponent<CTransform>().velocity.y < 0.0f)
		m_player->getComponent<CState>().state = "AIR";

	// If player is moving downwards they can no longer jump
	if (m_player->getComponent<CTransform>().velocity.y > 0.0f) {
		m_player->getComponent<CInput>().canJump = false;
		m_player->getComponent<CState>().state = "AIR";
	}
}

void Scene_Play::enemyMovement() 
{
	for (auto enemy : m_entityManager.getEntities("Enemy"))
	{
		if (enemy->getComponent<CAi>().behaviour == "Jumping")
		{
			// TODO set up movement for a jumping ai
			Vec2 enemyVelocity(enemy->getComponent<CTransform>().velocity.x, enemy->getComponent<CTransform>().velocity.y);

			if (enemy->getComponent<CInput>().up)
			{
				enemyVelocity.y += enemy->getComponent<CInput>().canJump ? -0.6f : 0;
			}

			enemy->getComponent<CTransform>().velocity = enemyVelocity;
		}

		if (enemy->getComponent<CAi>().behaviour == "BackAndForth")
		{
			// TODO set up movement for a jumping ai
			Vec2 enemyVelocity(enemy->getComponent<CTransform>().velocity.x, enemy->getComponent<CTransform>().velocity.y);

			if (enemy->getComponent<CInput>().left)
			{
				enemyVelocity.x += -0.2f;
				enemy->getComponent<CTransform>().scale.x = -1 * std::abs(enemy->getComponent<CTransform>().scale.x);
			}

			if (enemy->getComponent<CInput>().right)
			{
				enemyVelocity.x += 0.2f;
				enemy->getComponent<CTransform>().scale.x = std::abs(enemy->getComponent<CTransform>().scale.x);
			}

			enemy->getComponent<CTransform>().velocity = enemyVelocity;
		}

		// Clamp player velocity in either direction
		if (enemy->getComponent<CTransform>().velocity.y > 6.0f)
		{
			enemy->getComponent<CTransform>().velocity.y = 6.0f;
		}

		if (enemy->getComponent<CTransform>().velocity.x > 4.0f)
		{
			enemy->getComponent<CTransform>().velocity.x = 4.0f;
		}

		if (enemy->getComponent<CTransform>().velocity.y < -6.0f)
		{
			enemy->getComponent<CTransform>().velocity.y = -6.0f;
			enemy->getComponent<CInput>().canJump = false;
		}

		if (enemy->getComponent<CTransform>().velocity.x < -4.0f)
		{
			enemy->getComponent<CTransform>().velocity.x = -4.0f;
		}

		// If player is moving upwards they are in the air
		if (enemy->getComponent<CTransform>().velocity.y < 0.0f)
		{
			enemy->getComponent<CState>().state = "AIR";
		}

		// If player is moving downwards they can no longer jump
		if (enemy->getComponent<CTransform>().velocity.y > 0.0f)
		{
			enemy->getComponent<CInput>().canJump = false;
			enemy->getComponent<CState>().state = "AIR";
		}
	}
}

void Scene_Play::sMovement()
{
	playerMovement();

	enemyMovement();

	for (auto entity : m_entityManager.getEntities()) 
	{
		/*if (entity->tag() != "player" && entity->tag() != "enemy")
			continue;*/

		if (entity->hasComponent<CGravity>()) 
		{
			entity->getComponent<CTransform>().velocity.y += entity->getComponent<CGravity>().gravity;
		}
		entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
		entity->getComponent<CTransform>().pos += entity->getComponent<CTransform>().velocity;

		if (entity->getComponent<CTransform>().pos.y > height() + m_gridSize.y * 2.f)
		{
			entity->destroy();
			
			if (entity->tag() == "Player")
			{
				spawnPlayer(3.f, 3.f);
			}
		}
	}
}

void Scene_Play::sLifespan()
{
	// TODO: Check lifespan of entities that have them, and destroy them if they go over
	//
	// for all entities
	// if entity has no lifespan component, skip
	// if entity has >0 remaining lifespan, subtract 1
	// if it has lifespan and is alive
	//	scale its alpha channel properly
	// if it has lifespan and its time is up
	//	destory the entity
	for (auto& entity : m_entityManager.getEntities()) {
		if (entity->hasComponent<CLifeSpan>()) {

			entity->getComponent<CLifeSpan>().frameCreated++;

			if (entity->getComponent<CLifeSpan>().lifespan < entity->getComponent<CLifeSpan>().frameCreated)
				entity->destroy();
		}
	}
}

void Scene_Play::sCollision()
{
	//	REMEMBER: SFML's (90,0) position is on the TOP-LEFT corner
	//	This means jumping will have a negative y-component
	//	and gravity will have a positive y-component
	//	Also, something BELOW something else will have a y value GREATER than it
	//	Also, something ABOVE something else will have a y value LESS than it

	Physics physics;

	// TODO: Implement Physics::GetOverlap() function, use it inside this function

	for (const auto& bullet : m_entityManager.getEntities("Bullet")) {
		for (const auto& entity : m_entityManager.getEntities()) {

			if (entity->tag() == "Player" || entity->tag() == "Bullet")
				continue;

			if (!canCollide(bullet, entity))
				continue;

			Vec2 prevCollision = physics.GetPreviousOverlap(entity, bullet);


			// Must be colliding in both x and y to destroy bullet
			if (prevCollision.x > 0.f && prevCollision.y > 0.f)
			{
				std::cout << "bullet collided with " << entity->tag() << " , bullet will be destroyed\n";
				bullet->destroy();

				if (entity->tag() == "Enemy")
				{
					entity->destroy();
				}
			}
		}
	}

	bool hasCollided = false;

	// Loop through tiles checking collisions for player
	for (const auto& entity : m_entityManager.getEntities("Tile"))
	{		
		if (!m_player->isActive())
			continue;

		if (!canCollide(entity, m_player))
			continue;

		Vec2 collision = physics.GetOverlap(entity, m_player);

		if (collision.x > 0.f && collision.y > 0.f) 
		{

			Vec2 prevCollision = physics.GetPreviousOverlap(entity, m_player);

			if (m_player->isActive())
			{
				// check collision above player

				if (entity->tag() == "Tile")
				{
					Vec2 entityTilePos = pixelToGrid(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y);
					Vec2 playerTilePos = pixelToGrid(m_player->getComponent<CTransform>().pos.x, m_player->getComponent<CTransform>().pos.y);

					/*std::cout << "Player is in tile (" << playerTilePos.x << ", " << playerTilePos.y << ")\n";
					std::cout << "Player collided with tile at (" << entityTilePos.x << ", " << entityTilePos.y << ")\n";*/

					if (prevCollision.y <= 0.f &&
						(m_player->getComponent<CTransform>().pos.y > entity->getComponent<CTransform>().pos.y))
					{
						m_player->getComponent<CTransform>().pos.y = m_player->getComponent<CTransform>().prevPos.y;

						m_player->getComponent<CTransform>().velocity.y = 0;
					}

					// check collision below player

					if (prevCollision.y <= 0.f &&
						(m_player->getComponent<CTransform>().pos.y < entity->getComponent<CTransform>().pos.y))
					{
						if (std::abs(m_player->getComponent<CTransform>().pos.x - entity->getComponent<CTransform>().pos.x) <= m_player->getComponent<CBoundingBox>().size.x - 1.0f)
							{
								m_player->getComponent<CState>().state = "GROUND";

								m_player->getComponent<CTransform>().velocity.y = 0;

								m_player->getComponent<CTransform>().pos.y = m_player->getComponent<CTransform>().prevPos.y;

								m_player->getComponent<CInput>().canJump = true;
							}
					}

					// check collision to the right of player
					if (prevCollision.x <= 0.f &&
						(m_player->getComponent<CTransform>().pos.x < entity->getComponent<CTransform>().pos.x))
					{
						if (std::abs(m_player->getComponent<CTransform>().pos.y - entity->getComponent<CTransform>().pos.y) <= m_player->getComponent<CBoundingBox>().size.y - 1.0f)
						{
							m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CTransform>().prevPos.x;

							m_player->getComponent<CTransform>().velocity.x = 0;
						}

						if (m_player->getComponent<CState>().state != "GROUND")
						{
							m_player->getComponent<CState>().state = "WALLSLIDE";
						}
					}

					// check collision to the left of player
					if (prevCollision.x <= 0.f &&
						(m_player->getComponent<CTransform>().pos.x > entity->getComponent<CTransform>().pos.x))
					{
						if (std::abs(m_player->getComponent<CTransform>().pos.y - entity->getComponent<CTransform>().pos.y) <= m_player->getComponent<CBoundingBox>().size.y - 1.0f)
						{
							m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CTransform>().prevPos.x;

							m_player->getComponent<CTransform>().velocity.x = 0;
						}

						if (m_player->getComponent<CState>().state != "GROUND")
						{
							m_player->getComponent<CState>().state = "WALLSLIDE";
						}
					}
				}

				if (entity->tag() == "Hazard")
				{
					m_player->destroy();
					spawnPlayer(3.f, 3.f);
				}
			}
		}
		/*else
		{
			m_player->getComponent<CState>().state = "AIR";
		}*/

		// Loop through enemies to do collision on
	}

	// Loop for checking collisions of enemies
	for (const auto& enemy : m_entityManager.getEntities("Enemy"))
	{
		for (const auto& otherEntity : m_entityManager.getEntities())
		{
			if (otherEntity->tag() == "Enemy")
				continue;

			if (!canCollide(enemy, otherEntity))
				continue;

			Vec2 collision = physics.GetOverlap(enemy, otherEntity);

			if (collision.x > 0.f && collision.y > 0.f && enemy->isActive() && otherEntity->isActive())
			{
				Vec2 prevCollision = physics.GetPreviousOverlap(enemy, otherEntity);

				if (otherEntity->tag() == "Tile")
				{
					if (prevCollision.y <= 0.f &&
						(enemy->getComponent<CTransform>().pos.y > otherEntity->getComponent<CTransform>().pos.y))
					{
						enemy->getComponent<CTransform>().pos.y = enemy->getComponent<CTransform>().prevPos.y;

						enemy->getComponent<CTransform>().velocity.y = 0;

					}

					// check collision below enemy

					if (prevCollision.y <= 0.f &&
						(enemy->getComponent<CTransform>().pos.y < otherEntity->getComponent<CTransform>().pos.y))
					{
						if (std::abs(enemy->getComponent<CTransform>().pos.x - otherEntity->getComponent<CTransform>().pos.x) <= enemy->getComponent<CBoundingBox>().size.x - 1.0f)
						{
							enemy->getComponent<CState>().state = "GROUND";

							enemy->getComponent<CTransform>().velocity.y = 0;

							enemy->getComponent<CTransform>().pos.y = enemy->getComponent<CTransform>().prevPos.y;

							enemy->getComponent<CInput>().canJump = true;
						}
					}

					// check collision to the right of enemy
					if (prevCollision.x <= 0.f &&
						(enemy->getComponent<CTransform>().pos.x < otherEntity->getComponent<CTransform>().pos.x))
					{
						if (std::abs(enemy->getComponent<CTransform>().pos.y - otherEntity->getComponent<CTransform>().pos.y) <= enemy->getComponent<CBoundingBox>().size.y - 1.0f)
						{
							enemy->getComponent<CTransform>().pos.x = enemy->getComponent<CTransform>().prevPos.x;

							if (enemy->getComponent<CAi>().behaviour == "BackAndForth")
							{
								enemy->getComponent<CInput>().right = false;
								enemy->getComponent<CInput>().left = true;
							}
						}

						//enemy->getComponent<CTransform>().scale.x = -1.f * enemy->getComponent<CTransform>().scale.x;

						if (enemy->getComponent<CState>().state != "GROUND")
						{
							enemy->getComponent<CState>().state = "WALLSLIDE";
						}
					}

					// check collision to the left of enemy
					if (prevCollision.x <= 0.f &&
						(enemy->getComponent<CTransform>().pos.x > otherEntity->getComponent<CTransform>().pos.x))
					{
						if (std::abs(enemy->getComponent<CTransform>().pos.y - otherEntity->getComponent<CTransform>().pos.y) <= enemy->getComponent<CBoundingBox>().size.y - 1.0f)
						{
							enemy->getComponent<CTransform>().pos.x = enemy->getComponent<CTransform>().prevPos.x;

							if (enemy->getComponent<CAi>().behaviour == "BackAndForth")
							{
								enemy->getComponent<CInput>().left = false;
								enemy->getComponent<CInput>().right = true;
							}
						}

						if (enemy->getComponent<CState>().state != "GROUND")
						{
							enemy->getComponent<CState>().state = "WALLSLIDE";
						}
					}
				}

				if (otherEntity->tag() == "Player")
				{
					otherEntity->destroy();
					spawnPlayer(3, 3);
				}
			}
		}
	}

	// TODO: Implement bullet / tile collisions
	//	Destroy the tile if it has a Brick animation
	// TODO: Implement player / tile collisions and resolutions
	//	Update the CState component of the player to store whether
	//	it is currently on the ground or in the air. This will be
	//	used by the Animation system
	// TODO: Check to see if the player has fallen down a hole (y > height())
	// TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action& action)
{
	if (action.type() == "START") {

		if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures;  }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision;  }
		else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid;  }
		else if (action.name() == "PAUSE") { setPaused(!m_paused);  }
		/*else if (action.name() == "QUIT") { onEnd();  }*/

		if (action.name() == "MOVE_LEFT") 
		{ 
			m_player->getComponent<CInput>().left = true;
		}

		if (action.name() == "MOVE_RIGHT")
		{
			m_player->getComponent<CInput>().right = true;
		}

		if (action.name() == "MOVE_UP")
		{
			m_player->getComponent<CInput>().up = true;
		}

		if (action.name() == "MOVE_DOWN")
		{
			m_player->getComponent<CInput>().down = true;
		}

		if (action.name() == "SHOOT")
		{
			spawnBullet(m_player);
		}

		if (action.name() == "SET_TILE")
		{
			setTile();
		}
		if (action.name() == "REMOVE_TILE")
		{
			removeTile();
		}
	}
	else if (action.type() == "END") {

		if (action.name() == "MOVE_LEFT")
		{
			m_player->getComponent<CInput>().left = false;
		}

		if (action.name() == "MOVE_RIGHT")
		{
			m_player->getComponent<CInput>().right = false;
		}

		if (action.name() == "MOVE_UP")
		{
			m_player->getComponent<CInput>().up = false;
			m_player->getComponent<CInput>().canJump = false;
		}

		if (action.name() == "MOVE_DOWN")
		{
			m_player->getComponent<CInput>().down = false;
		}

		if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}

void Scene_Play::sAnimation()
{
	// TODO: Complete the Animation class code first 

	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->tag() != "Enemy" && entity->tag() != "Player")
			continue;

		if (entity->getComponent<CState>().state == "AIR")
		{
			if (entity->getComponent<CTransform>().velocity.y < 0)
			{
				entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(entity->tag() + "Jump"), true);
			}

			if (entity->getComponent<CTransform>().velocity.y > 0)
			{
				entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(entity->tag() + "Fall"), true);
			}
		}

		if (entity->getComponent<CState>().state == "GROUND")
		{
			if (entity->getComponent<CTransform>().velocity.x != 0
				&& entity->getComponent<CAnimation>().animation.getName() != entity->tag() + "Run")
			{
				entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(entity->tag() + "Run"), true);
			}

			if (entity->getComponent<CTransform>().velocity.x == 0
				&& entity->getComponent<CAnimation>().animation.getName() != entity->tag() + "Idle")
			{
				entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(entity->tag() + "Idle"), true);
			}
		}

		if (entity->getComponent<CState>().state == "WALLSLIDE")
		{
			entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(entity->tag() + "WallSlide"), true);
		}

		/*entity->getComponent<CTransform>().scale.x = entity->getComponent<CTransform>().velocity.x >= 0 ?
			std::abs(entity->getComponent<CTransform>().scale.x) : -1.f * std::abs(entity->getComponent<CTransform>().scale.x);*/
	}

	// TODO: set the animation of the player based on its CState component
	// TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
	//	if the animation is not repeated, and it has ended, destroy the entity
	for (auto entity : m_entityManager.getEntities())
	{
		if (!entity->hasComponent<CAnimation>())
			continue;

		entity->getComponent<CAnimation>().animation.update();

		// If an animation that is set to repeat has ended, restart the animation
		if (entity->getComponent<CAnimation>().animation.hasEnded() &&
			entity->getComponent<CAnimation>().repeat)
		{
			entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(entity->getComponent<CAnimation>().animation.getName()), true);
		}
	}
}

void Scene_Play::onEnd()
{
	// TODO: When the scene ends, change back to the MENU scene
	// use m_game->changeScene(correct params);

	// These actions are here to stop the player from continuously moving
	// when holding a movement key down and exiting the play scene
	sDoAction(Action("MOVE_LEFT", "END"));
	sDoAction(Action("MOVE_RIGHT", "END"));
	sDoAction(Action("MOVE_DOWN", "END"));
	sDoAction(Action("MOVE_UP", "END"));

	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Play::sRender()
{
	// color the background darker so you know that the game is paused
	if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
	else { m_game->window().clear(sf::Color(50, 50, 150)); }

	// set the viewport of the window to be centered on the player if it's far enough right
	auto& pPos = m_player->getComponent<CTransform>().pos;
	auto windowSize = m_game->window().getSize().x;

	sf::View view = m_game->window().getView();

	auto& cameraCenterX = view.getCenter().x;
	auto& cameraCenterY = view.getCenter().y;

	auto cameraLeftX = view.getCenter().x - (static_cast<float>(width()) / 2.f);
	auto cameraTopY = view.getCenter().y + (static_cast<float>(height()) / 2.f);

	auto cameraRightX = cameraLeftX + width();
	auto cameraBottomY = cameraTopY - height();

	// Player on right side of the screen
	if (pPos.x > view.getCenter().x) 
	{
		while (pPos.x - cameraCenterX > m_game->window().getSize().x / 8.0f) 
		{
			view.setCenter(cameraCenterX + 1.0f, cameraCenterY);
			m_game->window().setView(view);
		}
	}
	// Player on the left side of the screen
	else 
	{
		while (cameraCenterX - pPos.x > m_game->window().getSize().x / 8.0f)
		{
			view.setCenter(cameraCenterX - 1.0f, cameraCenterY);
			m_game->window().setView(view);
		}
	}

	// Player is below screen
	//if (pPos.y > view.getCenter().y)
	//{
	//	while (pPos.y - cameraCenterY > m_game->window().getSize().y / 3.0f)
	//	{
	//		view.setCenter(cameraCenterX, cameraCenterY + 1.0f);
	//		m_game->window().setView(view);
	//	}
	//}
	//// Player is above screen
	//else
	//{
	//	while (cameraCenterY - pPos.y > m_game->window().getSize().y / 3.0f)
	//	{
	//		view.setCenter(cameraCenterX, cameraCenterY - 1.0f);
	//		m_game->window().setView(view);
	//	}
	//}

	// draw all Entity textures / animations
	if (m_drawTextures) {

		// TODO: Set the proper clang compiler to build for C++20 allowing this

		/*auto filterByBounds = (m_entityManager.getEntities()) | std::views::filter([cameraLeftX, cameraRightX, cameraTopY, cameraBottomY](const auto& entity) {
			return entity->template getComponent<CTransform>().pos.x >= cameraLeftX && entity->template getComponent<CTransform>().pos.x <= cameraRightX 
				&& entity->template getComponent<CTransform>().pos.y >= cameraBottomY && entity->template getComponent<CTransform>().pos.y <= cameraTopY;
		});*/

		for (const auto& entity : m_entityManager.getEntities()) {

			if (!(entity->template getComponent<CTransform>().pos.x >= cameraLeftX && entity->template getComponent<CTransform>().pos.x <= cameraRightX
				&& entity->template getComponent<CTransform>().pos.y >= cameraBottomY && entity->template getComponent<CTransform>().pos.y <= cameraTopY))
				continue;
			
			auto& transform = entity->getComponent<CTransform>();

			if (entity->hasComponent<CAnimation>()) {
				auto& animation = entity->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	// draw all Entity collision bounding boxes with a rectangleshape
	if (m_drawCollision) {
		for (const auto& entity : m_entityManager.getEntities()) {

			if (!(entity->template getComponent<CTransform>().pos.x >= cameraLeftX && entity->template getComponent<CTransform>().pos.x <= cameraRightX
				&& entity->template getComponent<CTransform>().pos.y >= cameraBottomY && entity->template getComponent<CTransform>().pos.y <= cameraTopY))
				continue;

			if (entity->hasComponent<CBoundingBox>()) {
				auto& box = entity->getComponent<CBoundingBox>();
				auto& transform = entity->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}

	// draw the grid so you can easily debug
	if (m_drawGrid) {

		auto startGridPos = pixelToGrid(cameraLeftX, cameraBottomY);

		// Now looping from start of the left-most viewable grid, to the furthest right viewable grid
		for (float x = startGridPos.x * m_gridSize.x; x < cameraRightX; x += m_gridSize.x) {
			drawLine(Vec2(x, cameraBottomY), Vec2(x, cameraTopY));
		}

		// Now looping from start of the lowest viewable grid, to the highest up viewable grid
		for (float y = ((startGridPos.y * m_gridSize.y) * -1.f) + static_cast<float>(height()); y < cameraTopY; y += m_gridSize.y) {
			drawLine(Vec2(cameraLeftX, y), Vec2(cameraRightX, y));

			for (float x = startGridPos.x * m_gridSize.x; x < cameraRightX; x += m_gridSize.x) {
				const auto& gridPos = pixelToGrid(x, y);
				std::string xCell = std::to_string(static_cast<int>(gridPos.x));
				std::string yCell = std::to_string(static_cast<int>(gridPos.y));
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, y - m_gridSize.y + 2);
				m_gridText.setFillColor(sf::Color::White);
				m_game->window().draw(m_gridText);
			}
		}
	}

	m_game->window().display();
}

void Scene_Play::drawLine(const Vec2& p1, const Vec2& p2)
{
	// TODO: drawLine from p1 to p2
	sf::VertexArray line(sf::Lines, 2);

	line[0].position = sf::Vector2f(p1.x, p1.y); // Set the position of the first vertex
	line[0].color = sf::Color::White; // Set the color of the first vertex

	line[1].position = sf::Vector2f(p2.x, p2.y); // Set the position of the second vertex
	line[1].color = sf::Color::White; // Set the color of the second vertex

	// Assuming you have an sf::RenderWindow object called 'window'
	m_game->window().draw(line);
}

size_t Scene_Play::width() const
{
	// TODO: get width of window
	return size_t(m_game->window().getSize().x);
}

size_t Scene_Play::height() const
{
	// TODO: get height of window
	return size_t(m_game->window().getSize().y);
}