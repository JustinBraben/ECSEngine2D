#include "Scene.hpp"

void Scene::setPaused(bool paused)
{
    m_paused = paused;
}

void Scene::setDelayMove()
{
}

Scene::Scene()
{
}

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

size_t Scene::width() const
{
    // TODO: get width of window
    return size_t(100);
}

size_t Scene::height() const
{
    // TODO: get height of window
    return size_t(100);
}

const ActionMap& Scene::getActionMap() const {
	return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
    // TODO: drawLine from p1 to p2
}
