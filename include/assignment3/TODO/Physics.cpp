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

	auto& aBox = a->getComponent<CBoundingBox>();
	auto& aTransform = a->getComponent<CTransform>();
	auto& bBox = b->getComponent<CBoundingBox>();
	auto& bTransform = b->getComponent<CTransform>();

	Vec2 delta = { std::abs(aTransform.prevPos.x - bTransform.prevPos.x) ,
						std::abs(aTransform.prevPos.y - bTransform.prevPos.y) };

	auto overLapX = aBox.halfSize.x + bBox.halfSize.x - delta.x;
	auto overLapY = aBox.halfSize.y + bBox.halfSize.y - delta.y;

	return Vec2(overLapX, overLapY);
}