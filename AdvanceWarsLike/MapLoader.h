#ifndef MAPLOADER_H_
#define MAPLOADER_H_
#pragma once

#include <iostream>

#include "pugixml.hpp"
#include "Map.h"

class MapLoader
{
public:
	MapLoader();
	virtual ~MapLoader();

	bool printError(pugi::xml_parse_result &result) const;
	TileType getTileType(const sf::String &name) const;
	bool loadLayer(const pugi::xml_node data, std::vector<std::vector<Tile>> &tiles, const sf::Vector2u &size) const;
	bool loadBuildings(const pugi::xml_node data, std::vector<std::vector<IBuilding *>> &tiles, const sf::Vector2u &size, sf::Uint8 &qg) const;
	bool loadMap(Map &map, const char *fileName);

private:
	std::map<sf::Uint32, sf::String> _tiles;
};
#endif // MAPLOADER_H_
