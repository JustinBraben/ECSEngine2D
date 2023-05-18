#pragma once
// Load once, use often
#include "Animation.hpp"
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
class Assets {

private:
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, Animation> m_animations;
	std::map<std::string, sf::Sound> m_sounds;
	std::map<std::string, sf::Font> m_fonts;

public:

	void addTexture(const std::string& name, const std::string& path);
	void addAnimation(const std::string& name, Animation&& animation);
	void addSound(const std::string& name, const std::string& path);
	void addFont(const std::string& name, const std::string& path);

	const sf::Texture& getTexture(const std::string& name) const;
	const Animation& getAnimation(const std::string& name) const;
	const sf::Sound& getSound(const std::string& name) const;
	const sf::Font& getFont(const std::string& name) const;
};