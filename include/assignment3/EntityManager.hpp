#pragma once

#include "Entity.hpp"
#include <iostream>
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

// Don't need this, but up to you
typedef std::map<std::string, EntityVec> EntityMap;  

class EntityManager {

	EntityVec m_entities;			// all entities
	EntityVec m_toAdd;				// entities to add next update
	EntityMap m_entityMap;			// map from entity tag to vectors
	size_t m_totalEntities = 0;		// total entities created

	// helper function to avoid repeated code
	void removeDeadEntities(EntityVec& vec);

public:
	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);
};