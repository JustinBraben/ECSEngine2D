#include "../Physics.hpp"
#include "Components.hpp"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {

	// TODO: return the overlap rectangle size of the bounding boxes of entity a and b
	if (!a->hasComponent<CTransform>()) 
	{
		return Vec2(0, 0);
	}
	
	if (!b->hasComponent<CTransform>()) 
	{
		return Vec2(0, 0);
	}

	if (!a->hasComponent<CBoundingBox>())
	{
		return Vec2(0, 0);
	}

	if (!b->hasComponent<CBoundingBox>())
	{
		return Vec2(0, 0);
	}

	auto& aBox = a->getComponent<CBoundingBox>();
	auto& aTransform = a->getComponent<CTransform>();
	auto& bBox = b->getComponent<CBoundingBox>();
	auto& bTransform = b->getComponent<CTransform>();

	/*rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
	rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
	rect.setPosition(transform.pos.x, transform.pos.y);
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineColor(sf::Color(255, 255, 255, 255));
	rect.setOutlineThickness(1);
	m_game->window().draw(rect);*/

	Vec2 delta = { std::abs(aTransform.pos.x - bTransform.pos.x) ,
						std::abs(aTransform.pos.y - bTransform.pos.y) };

	auto overlapX = aBox.halfSize.x + bBox.halfSize.x - delta.x;
	auto overlapY = aBox.halfSize.y + bBox.halfSize.y - delta.y;

	auto overlap = Vec2(overlapX, overlapY);

	return overlap;
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {

	// TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
	// previous overlap uses the entity's previous position

	if (!a->hasComponent<CTransform>())
	{
		return Vec2(0, 0);
	}

	if (!b->hasComponent<CTransform>())
	{
		return Vec2(0, 0);
	}

	if (!a->hasComponent<CBoundingBox>())
	{
		return Vec2(0, 0);
	}

	if (!b->hasComponent<CBoundingBox>())
	{
		return Vec2(0, 0);
	}

	Vec2 delta = { std::abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x) ,
						std::abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y) };

	auto overLapX = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x - delta.x;
	auto overLapY = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y - delta.y;

	return Vec2(overLapX, overLapY);
}