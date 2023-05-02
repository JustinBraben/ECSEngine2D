#include "Vec2.hpp"
#include <cmath>

Vec2::Vec2() {

}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin) {}

Vec2 Vec2::operator + (const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float val) const {
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float val) const {
	return Vec2(x * val, y * val);
}

void Vec2::operator += (const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator /= (const float val) {
	x /= val;
	y /= val;
}

void Vec2::operator *= (const float val) {
	x *= val;
	y *= val;
}

float Vec2::dist(const Vec2& rhs) const {
	return std::sqrt((rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y));
}