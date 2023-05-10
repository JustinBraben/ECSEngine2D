#include "../Animation.hpp"
#include <cmath>

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0) {}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
	: m_name(name), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed)
{
	m_size = Vec2(static_cast<float>(t.getSize().x) / frameCount, static_cast<float>(t.getSize().y));
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, dependning on its speed
// animation loops when it reaches the end
void Animation::update()
{
}
