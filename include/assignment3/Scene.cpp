#include "Scene.hpp"
#include "Scene.hpp"

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine)
{
    // Constructor implementation
    // You may initialize any member variables or perform other necessary actions here
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
    m_actionMap[inputKey] = actionName;
}

const ActionMap& Scene::getActionMap() const {
	return m_actionMap;
}