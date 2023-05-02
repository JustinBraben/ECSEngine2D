#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2() {}
	Vec2(float xIn, float yIn) :
		x(xIn), y(yIn) {}

	bool operator == (const Vec2& rhs) {
		return (rhs.x == this->x && rhs.y == this->y);
	}

	bool operator != (const Vec2& rhs) {
		return (rhs.x != this->x || rhs.y != this->y);
	}

	Vec2 operator + (const Vec2& rhs) const {
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator - (const Vec2& rhs) const {
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator * (const Vec2& rhs) const {
		return Vec2(x * rhs.x, y * rhs.y);
	}

	Vec2 operator / (const Vec2& rhs) const {
		return Vec2(x / rhs.x, y / rhs.y);
	}

	Vec2& add(const Vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2& scale(float s) {
		x *= s;
		y *= s;
		return *this;
	}

	Vec2& rotate(float deg) {
		return *this;
	}

	float dist(const Vec2& v) const {
		return std::sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y));
	}
private:
	
};