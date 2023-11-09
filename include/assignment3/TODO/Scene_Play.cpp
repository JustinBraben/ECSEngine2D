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

			if (tileType == "Ground") {
			auto brick = m_entityManager.addEntity("tile");
			auto& assets = m_game->getAssets();
			auto& animationOryxBrick = m_game->getAssets().getAnimation("OryxBrick");
			brick->addComponent<CAnimation>(animationOryxBrick, true);
			brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));
			brick->addComponent<CBoundingBox>(Vec2(64.0f, 64.0f));
			}

			if (tileType == "Question") {
				auto question = m_entityManager.addEntity("tile");
				auto& assets = m_game->getAssets();
				auto& animationOryxBrick = m_game->getAssets().getAnimation("OryxQuestion");
				question->addComponent<CAnimation>(animationOryxBrick, true);
				question->addComponent<CTransform>(gridToMidPixel(gridX, gridY, question));
				question->addComponent<CBoundingBox>(Vec2(64.0f, 64.0f));
			}
		}
	}

	// NOTE: all of the code below is sample code which shows you hot to
	//	set up and use entities with the new syntax, it should be removed

	spawnPlayer();

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

void Scene_Play::spawnPlayer()
{
	// here is a sample player entity which you can use to construct other entities
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("PlayerIdle"), true);
	m_player->addComponent<CTransform>(Vec2(224, 352));
	m_player->addComponent<CGravity>(0.1f);
	m_player->addComponent<CBoundingBox>(Vec2(64.0f, 64.0f));
	m_player->addComponent<CState>("AIR");

	// TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	// TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
	auto m_bullet = m_entityManager.addEntity("bullet");
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

void Scene_Play::update()
{
	m_entityManager.update();

	// TODO: implement pause functionality

	sMovement();
	sLifespan();
	sCollision();
	sAnimation();
	sRender();
}

void Scene_Play::sMovement()
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



	for (auto entity : m_entityManager.getEntities()) 
	{
		if (entity->hasComponent<CGravity>()) 
		{
			entity->getComponent<CTransform>().velocity.y += entity->getComponent<CGravity>().gravity;
		}
		entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
		entity->getComponent<CTransform>().pos += entity->getComponent<CTransform>().velocity;
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

	for (auto bullet : m_entityManager.getEntities("bullet")) {
		for (auto entity : m_entityManager.getEntities()) {
			if (entity->tag() == "player" || entity->tag() == "bullet")
				continue;

			Vec2 prevCollision = physics.GetPreviousOverlap(entity, bullet);


			// Must be colliding in both x and y to destroy bullet
			if (prevCollision.x > 0.f && prevCollision.y > 0.f)
			{
				std::cout << "bullet collided with " << entity->tag() << " , bullet will be destroyed\n";
				bullet->destroy();
			}
		}
	}

	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->tag() == "player")
			continue;

		// Skip collision detection on entities that are too far away to possibly collide with Player
		if (std::abs(entity->getComponent<CTransform>().pos.x - m_player->getComponent<CTransform>().pos.x) > m_player->getComponent<CBoundingBox>().size.x)
			continue;

		if (std::abs(entity->getComponent<CTransform>().pos.y - m_player->getComponent<CTransform>().pos.y) > m_player->getComponent<CBoundingBox>().size.y)
			continue;

		Vec2 collision = physics.GetOverlap(entity, m_player);

		if (collision.x > 0.f && collision.y > 0.f) {

			Vec2 prevCollision = physics.GetPreviousOverlap(entity, m_player);

			// TODO: check for collisions with tiles above the player
			// set canJump to false if a collision above player was detected
			if (prevCollision.y <= 0.f &&
				(m_player->getComponent<CTransform>().pos.y > entity->getComponent<CTransform>().pos.y))
			{
				m_player->getComponent<CTransform>().velocity.y = 0;

				m_player->getComponent<CInput>().canJump = false;

				// Debug collision
				//std::cout << "collided with entity above player\n";
			}

			// TODO: check collision for below player
			if (prevCollision.y <= 0.f &&
				(m_player->getComponent<CTransform>().pos.y < entity->getComponent<CTransform>().pos.y))
			{
				m_player->getComponent<CState>().state = "GROUND";

				m_player->getComponent<CTransform>().pos.y = m_player->getComponent<CTransform>().prevPos.y;

				m_player->getComponent<CTransform>().velocity.y = 0;

				m_player->getComponent<CInput>().canJump = true;

				// Debug collision
				//std::cout << "collided with entity below player\n";
			}

			if (prevCollision.y <= 0.f &&
				std::abs(m_player->getComponent<CTransform>().pos.y - entity->getComponent<CTransform>().pos.y) > m_player->getComponent<CBoundingBox>().halfSize.y)
			{
				/*m_player->getComponent<CState>().state = "GROUND";

				m_player->getComponent<CTransform>().pos.y = m_player->getComponent<CTransform>().prevPos.y;

				m_player->getComponent<CTransform>().velocity.y = 0;

				m_player->getComponent<CInput>().canJump = true;

				std::cout << "collided with entity below player\n";*/
			}
			else if (prevCollision.x <= 0.f &&
				std::abs(m_player->getComponent<CTransform>().pos.x - entity->getComponent<CTransform>().pos.x) > m_player->getComponent<CBoundingBox>().halfSize.x)
			{
				m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CTransform>().prevPos.x;

				m_player->getComponent<CTransform>().velocity.x = 0;
			}
		}

		if (m_player->getComponent<CInput>().canJump)
			m_player->getComponent<CState>().state = "GROUND";
		else
			m_player->getComponent<CState>().state = "AIR";
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
		else if (action.name() == "QUIT") { onEnd();  }

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
		}

		if (action.name() == "MOVE_DOWN")
		{
			m_player->getComponent<CInput>().down = false;
		}
	}
}

void Scene_Play::sAnimation()
{
	// TODO: Complete the Animation class code first 

	if (m_player->getComponent<CState>().state == "AIR")
	{
		// TODO: check if player is rising in the air
		// set jumping animation if so
		if (m_player->getComponent<CTransform>().velocity.y < 0)
		{
			m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("PlayerJump"), true);
		}

		// TODO: set falling animation
		if (m_player->getComponent<CTransform>().velocity.y > 0)
		{
			m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("PlayerFall"), true);
		}

	}

	if (m_player->getComponent<CState>().state == "GROUND")
	{
		if (m_player->getComponent<CTransform>().velocity.x != 0 
			&& m_player->getComponent<CAnimation>().animation.getName() != "PlayerRun")
		{
			m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("PlayerRun"), true);
		}

		if (m_player->getComponent<CTransform>().velocity.x == 0
			&& m_player->getComponent<CAnimation>().animation.getName() != "PlayerIdle")
		{
			m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("PlayerIdle"), true);
		}
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

	// Player on right side of the screen
	if (pPos.x > view.getCenter().x) 
	{
		while (pPos.x - cameraCenterX > m_game->window().getSize().x / 4.0f) 
		{
			view.setCenter(cameraCenterX + 1.0f, cameraCenterY);
			m_game->window().setView(view);
		}
	}
	// Player on the left side of the screen
	else 
	{
		while (cameraCenterX - pPos.x > m_game->window().getSize().x / 4.0f)
		{
			view.setCenter(cameraCenterX - 1.0f, cameraCenterY);
			m_game->window().setView(view);
		}
	}

	// Player is below screen
	if (pPos.y > view.getCenter().y)
	{
		while (pPos.y - cameraCenterY > m_game->window().getSize().y / 4.0f)
		{
			view.setCenter(cameraCenterX, cameraCenterY + 1.0f);
			m_game->window().setView(view);
		}
	}
	// Player is above screen
	else
	{
		while (cameraCenterY - pPos.y > m_game->window().getSize().y / 4.0f)
		{
			view.setCenter(cameraCenterX, cameraCenterY - 1.0f);
			m_game->window().setView(view);
		}
	}

	// draw all Entity textures / animations
	if (m_drawTextures) {
		for (auto entity : m_entityManager.getEntities()) {

			auto& transform = entity->getComponent<CTransform>();

			if (entity->hasComponent<CAnimation>()) {
				auto& animation = entity->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	// draw all Entity collision bounding boxes with a rectangleshape
	if (m_drawCollision) {
		for (auto entity : m_entityManager.getEntities()) {
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
		float leftX = m_game->window().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridSize.x;
		float nextGridX = leftX - static_cast<float>((static_cast<int>(leftX) % static_cast<int>(m_gridSize.x)));

		for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
			drawLine(Vec2(x, 0), Vec2(x, height()));
		}

		for (float y = 0; y < height(); y += m_gridSize.y) {
			drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

			for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
				std::string xCell = std::to_string(static_cast<int>(x) / static_cast<int>(m_gridSize.x));
				std::string yCell = std::to_string(static_cast<int>(y) / static_cast<int>(m_gridSize.y));
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
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