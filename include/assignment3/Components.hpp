#pragma once

#include "Vec2.hpp"
#include "Animation.hpp"
#include <SFML/Graphics.hpp>

class Component{
public:
	bool has = false;
};

class CTransform : public Component {
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 prevPos = { 0.0, 0.0 };
	Vec2 scale = { 64.0f / 24.0f, 64.0f / 24.0f};		// converting tile textures from 24x24 to 64x64
	Vec2 velocity{ 0.0, 0.0 };
	float angle = 0;

	CTransform() {}

	CTransform(const Vec2& p)
		: pos(p) {}

	CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
		: pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
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
	Vec2 size = { 64.0f, 64.0f};
	Vec2 halfSize = { 32.0f, 32.0f };
	CBoundingBox() {}
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
	bool canShoot = true;
	bool canJump = true;

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
	int lifespan = 0;
	int frameCreated = 0;

	CLifeSpan() {}

	CLifeSpan(int duration, int frame)
		: lifespan(duration), frameCreated(frame) {}
};

class CAnimation : public Component {
public:
	Animation animation;
	bool repeat = false;
	CAnimation() {}
	CAnimation(const Animation& animation, bool r)
		: animation(animation), repeat(r) {}
};

class CGravity : public Component {
public:
	float gravity = 0;
	CGravity() {}
	CGravity(float g) 
		: gravity(g) {}
};

class CState : public Component {
public:
	std::string state = "jumping";
	CState() {}
	CState(const std::string& s)
		: state(s) {}
};

class CAi : public Component {
public:
	std::string behaviour = "BackAndForth";
	CAi() {}
	CAi(std::string aiType)
		: behaviour(aiType) {}
};