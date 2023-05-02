#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Entity.hpp"
using EntityVec = std::vector<std::shared<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;
class EntityManager {
	EntityVec m_entities;
	EntityVec m_toAdd;			// Waiting room to check if entity can be added
	EntityMap m_entityMap;
	size_t m_totalEntities = 0;
public:
	EntityManager();

	// called at beginning of each frame by game engine
	// entities added will now be available to use this frame
	void update() {
		for (auto e : m_toAdd) {
			m_entities.push_back(e);
			m_entityMap[e->tag()].push_back(e);
		}

		for (auto e : m_entities) {
			// if e is dead, remove it from the m_entities
			// if e is dead, remove it from the  m_entityMap[e->tag()]
		}
		m_toAdd.clear();
	}

	/*std::shared_ptr<Entity> addEntity(const std::string& tag) {
		auto e = std::make_shared<Entity>(tag, m_totalEntities++);
		m_toAdd.push_back(e);
		return e;
	}*/

	std::shared_ptr<Entity> addEntity(const std::string& tag) {

		// create a new Entity
		auto e = std::make_shared<Entity>(tag, m_totalEntities++);

		// store it in the vector of all entities
		m_entities.push_back(e);

		// store it in the map of tag->entityVector
		m_entityMap[tag].push_back(e);

		// return the shared pointer pointing to that entity
		return e;
	}
	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);
};