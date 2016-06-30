#ifndef MAPLOADER_H_
#define MAPLOADER_H_
#pragma once

#include "pugixml.hpp"
#include "Map.h"

class MapLoader
{
public:
	MapLoader();
	virtual ~MapLoader();
	bool printError(pugi::xml_parse_result &result) const;
	bool loadLayer(const pugi::xml_node data, std::vector<std::vector<Tile>> &tiles, const sf::Vector2u &size) const;
	bool loadMap(Map &map, const char *fileName) const;
};
#endif // MAPLOADER_H_
