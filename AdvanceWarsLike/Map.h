#ifndef MAP_H_
#define MAP_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "ResourcesManager.h"
#include "IUnit.h"
#include "IBuilding.h"

enum TileType
{
	NONE = 0,
	GROUND = 1,
	WATER = 2,
	FOREST = 3,
	MOUNTAIN = 4,
	BUILDING = 5,
	ROAD = 6
};

struct Tile
{
	Tile() : type(NONE), tile("") {}
	Tile(TileType ctype, sf::String ctile) : type(ctype), tile(ctile) {}
	TileType type;
	sf::String tile;
};

class Map : public sf::Drawable, public sf::Transformable
{
	friend class MapLoader;
public:
	Map(ResourcesManager &resourcesManager, const sf::Font &font);
	virtual ~Map();

	void removeUnit(const sf::Vector2u &position);
	bool addUnit(IUnit *unit, sf::Vector2u position);
	IUnit *getUnit(const sf::Vector2u &position);
	sf::Vector2u getMapSize() const;
	sf::Vector2u getTileSize() const;
	sf::Int8 getTileMovement(TileType tile) const;
	const std::vector<std::vector<IBuilding *>> &getBuildings() const;
	const std::vector<std::vector<IUnit *>> &getUnits() const;
	TileType getTile(sf::Vector2i pos) const;
	const sf::Uint8 &getQG() const;
	bool canMove(const sf::Vector2u &unitPosition, const sf::Vector2u &position);
	void move(const sf::Vector2u &unitPosition, const sf::Vector2u &position);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	ResourcesManager &_resourcesManager;
	const sf::Font &_font;
	sf::Text _hp;
	std::map<sf::String, std::vector<std::vector<Tile>>> _tiles;
	std::vector<std::vector<IBuilding *>> _buildings;
	std::vector<std::vector<IUnit *>> _units;
	sf::Vector2u _tileSize;
	sf::Vector2u _size;
	sf::Uint8 _qg;
};

#endif // MAP_H_
