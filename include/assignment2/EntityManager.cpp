#include "EntityManager.hpp"

EntityManager::EntityManager()
	: m_entities(EntityVec()), m_toAdd(EntityVec()), m_entityMap(EntityMap()), m_totalEntities(0) {}

void EntityManager::update() {
	for (auto e : m_toAdd) {
		// add the entity to the main vector of entities
		m_entities.push_back(e);

		// store it in the map of tag->entityVector
		m_entityMap[e->tag()].push_back(e);
	}

	for (auto e : m_entities) {
		// if e is dead, remove it from the m_entities
		// if e is dead, remove it from the  m_entityMap[e->tag()]
	}

	m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
 
	// give each entity an increasing integer id
	// not using make shared because you cannot use
	// make_shared to make a shared_ptr to a private constructor
	// even through a friend class
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	// store it in the vector of toAdd entities
	// when update() is called it will handle it
	m_toAdd.push_back(e);

	// return the shared pointer pointing to that entity
	return e;
}