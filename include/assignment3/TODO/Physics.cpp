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

	Vec2 delta = { std::abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x) ,
						std::abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y) };

	auto overlapX = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x - delta.x;
	auto overlapY = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y - delta.y;

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