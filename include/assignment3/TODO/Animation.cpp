#include "Animation.hpp"
#include "../Animation.hpp"
#include <cmath>

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, const sf::IntRect& tRect)
	: m_name(name)
{
	m_size = Vec2(static_cast<float>(tRect.width), static_cast<float>(tRect.height));
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTexture(t);
	m_sprite.setTextureRect(tRect);
}

Animation::Animation(const std::string& name, const sf::Texture& t, const sf::IntRect& tRect, size_t frameCount, size_t speed, Vec2 size)
	: m_name(name), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed), m_size(size)
{
	m_size = Vec2(static_cast<float>(tRect.width), static_cast<float>(tRect.height));
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTexture(t);
	m_sprite.setTextureRect(tRect);
}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0) {}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed, Vec2 size)
	: m_name(name), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed), m_size(size)
{
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
{
	m_size = Vec2(static_cast<float>(t.getSize().x) / frameCount, static_cast<float>(t.getSize().y));
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update()
{
	// add the speed variable to the current frame
	m_currentFrame += m_speed;

	// TODO:	1) calculate the correct frame of animation to play based on currentFrame and speed
	//			2) set the texture rectange properly (see constructor sample)
	if (m_speed > 0)
	{
		m_size = Vec2(static_cast<float>(m_sprite.getTexture()->getSize().x) / m_frameCount, static_cast<float>(m_sprite.getTexture()->getSize().y));
		m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
		m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame / 8) * m_size.x, m_sprite.getTextureRect().top, m_size.x, m_size.y));
	}
}

bool Animation::hasEnded() const
{
	// TODO: detect when animation has ended (last frame was played) and return true
	if (m_currentFrame / 8 >= m_frameCount)
		return true;
	return false;
}

const std::string& Animation::getName() const
{
	return m_name;
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}
