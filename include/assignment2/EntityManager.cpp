#include "EntityManager.hpp"

EntityManager::EntityManager()
	: m_entities(EntityVec()), m_toAdd(EntityVec()), m_entityMap(EntityMap()), m_totalEntities(0) {}