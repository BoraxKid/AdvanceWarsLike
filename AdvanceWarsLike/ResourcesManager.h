#ifndef RESOURCESMANAGER_H_
#define RESOURCESMANAGER_H_
#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

#include "pugixml.hpp"
#include "AnimatedSprite.h"

class ResourcesManager
{
public:
	ResourcesManager();
	virtual ~ResourcesManager();
	void update(const sf::Time &elapsed);
	void loadTexture(sf::String textureName);
	void loadSprites(const char *file);
	AnimatedSprite &at(const sf::String &spriteName);

private:
	std::map<sf::String, sf::Texture> _textures;
	std::map<sf::String, AnimatedSprite> _sprites;
};

#endif // RESOURCESMANAGER_H_