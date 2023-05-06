#pragma once

#include "Vec2.hpp"
#include <SFML/Graphics.hpp>

class Component{};

class CTransform : public Component {
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 prevPos = { 0.0, 0.0 };
	Vec2 scale = { 1.0, 1.0 };
	Vec2 velocity{ 0.0, 0.0 };
	float angle = 0;

	CTransform(const Vec2& p, const Vec2& v, float a)
		: pos(p), velocity(v), angle(a) {}
};

class CShape : public Component {
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points) {
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}

	auto getFillColor() const { return circle.getFillColor(); }

	void setFillColor(const sf::Color& fill) { circle.setFillColor(fill); }

	void setOutlineColor(const sf::Color& outline) { circle.setOutlineColor(outline); }
};

class CBoundingBox : public Component {
public:
	Vec2 size;
	Vec2 halfSize;
	CBoundingBox(const Vec2& s)
		: size(s), halfSize(s.x / 2, s.y / 2) {}
};

class CCollision : public Component {
public:
	float radius = 0;
	CCollision(float r)
		: radius(r) {}
};

class CInput : public Component {
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;

	CInput() {}
};

class CScore : public Component {
public:
	int score = 0;
	CScore(int s)
		: score(s) {}
};

class CLifeSpan : public Component {
public:
	int remaining = 0;	// amount of lifespan remaining on the entity
	int total = 0;		// the toal initial amount of lifespan
	CLifeSpan(int total)
		: remaining(total), total(total) {}
};