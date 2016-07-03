#ifndef MAP_H_
#define MAP_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "ResourcesManager.h"
#include "IUnit.h"

typedef int Tile;

class Map : public sf::Drawable, public sf::Transformable
{
	friend class MapLoader;
public:
	Map(ResourcesManager &resourcesManager);
	virtual ~Map();

	void dump() const;

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	ResourcesManager &_resourcesManager;
	std::map<sf::String, std::vector<std::vector<Tile>>> _tiles;
	std::vector<std::vector<Tile>> _buildings;
	std::vector<std::vector<IUnit *>> _units;
	std::map<Tile, sf::String> _tilesNames;
	sf::Vector2u _tileSize;
	sf::Vector2u _size;
};

#endif // MAP_H_
