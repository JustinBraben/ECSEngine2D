#include "EntityManager.hpp"

EntityManager::EntityManager()
	: m_entities(EntityVec()), m_toAdd(EntityVec()), m_entityMap(EntityMap()), m_totalEntities(0) {}


EntityVec& EntityManager::getEntities()
{
	// TODO: insert return statement here
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	// TODO: insert return statement here
	return m_entityMap[tag];
}

void EntityManager::update() {


	for (auto& entity : m_toAdd) {
		// add the entity to the main vector of entities
		m_entities.emplace_back(entity);

		// store it in the map of tag->entityVector
		m_entityMap[entity->tag()].push_back(entity);
	}

	// even with emplace_back this still seems necessary
	m_toAdd.clear();

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	for (auto& [tag, EntityVec] : m_entityMap) {
		removeDeadEntities(EntityVec);
	}

}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	// TODO: remove all dead entities from the input vector
	// This lambda erases entities that are no longer active
	vec.erase(std::remove_if(vec.begin(), vec.end(), [](const auto& entity) {
		return !entity->m_active;
		}), vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {

	// give each entity an increasing integer id
	// not using make shared because you cannot use
	// make_shared to make a shared_ptr to a private constructor
	// even through a friend class
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	// store it in the vector of toAdd entities
	// when update() is called it will handle it
	m_toAdd.push_back(entity);

	// return the shared pointer pointing to that entity
	return entity;
}