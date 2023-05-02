#pragma once
#include "Components/CTransform.hpp"
#include "Components/CName.hpp"
#include "Components/CShape.hpp"
#include "Components/CBBox.hpp"
class Entity {
	const size_t m_id = 0;
	const std::string m_tag = "Default";
	bool m_alive = true;
public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CBBox> cBBox;
	Entity(const std::string& tag, size_t id)
	: m_tag(tag), m_id(id) {}
	void destroy() { m_alive = false; }
	const std::string& tag() { return m_tag; }
};