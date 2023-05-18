#include "Assets.hpp"
#include "Assets.hpp"
#include <iostream>

void Assets::addTexture(const std::string& name, const std::string& path) {
	sf::Texture texture;

	if (!texture.loadFromFile(path)) {
		std::cout << "texture from path " << path << " was not loaded\n";
	}

	m_textures[name] = texture;
}

void Assets::addAnimation(const std::string& name, const Animation& animation)
{
	m_animations[name] = animation;
}

void Assets::addSound(const std::string& name, const std::string& path)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path)) {
		std::cout << "sound from path " << path << " was not loaded\n";
	}

	sf::Sound sound;
	sound.setBuffer(buffer);
	m_sounds[name] = sound;
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	sf::Font font;

	if (!font.loadFromFile(path)) {
		std::cout << "font from path " << path << " was not loaded\n";
	}

	m_fonts[name] = font;
}

const sf::Texture& Assets::getTexture(const std::string& name) const
{
	return m_textures.at(name);
}

const Animation& Assets::getAnimation(const std::string& name) const
{
	return m_animations.at(name);
}

const sf::Sound& Assets::getSound(const std::string& name) const
{
	return m_sounds.at(name);
}

const sf::Font& Assets::getFont(const std::string& name) const
{
	return m_fonts.at(name);
}
