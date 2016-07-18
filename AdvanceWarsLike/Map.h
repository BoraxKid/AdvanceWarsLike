#ifndef MAP_H_
#define MAP_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "ResourcesManager.h"
#include "IUnit.h"

enum Tile
	{
		GROUND = 0,
		WATER = 1,
		FOREST = 2,
		MOUNTAIN = 3,
		TOWER_RED = 15,
		TOWER_BLUE = 30,
		TOWER_GREEN = 45,
		TOWER_YELLOW = 60,
		TOWER_BLACK = 75
	};

class Map : public sf::Drawable, public sf::Transformable
{
	friend class MapLoader;
public:
	Map(ResourcesManager &resourcesManager);
	virtual ~Map();

	void removeUnit(const sf::Vector2u &position);
	bool addUnit(IUnit *unit, sf::Vector2u position);
	IUnit *getUnit(const sf::Vector2u &position);
	sf::Vector2u getMapSize() const;
	sf::Vector2u getTileSize() const;
	bool canMove(const sf::Vector2u &unitPosition, const sf::Vector2u &position);
	void move(const sf::Vector2u &unitPosition, const sf::Vector2u &position);
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
