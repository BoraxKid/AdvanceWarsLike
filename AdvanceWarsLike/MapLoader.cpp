#include "MapLoader.h"
#include "Building.h"

MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
}

bool MapLoader::printError(pugi::xml_parse_result &result) const
{
	std::cerr << result.description() << std::endl;
	return (false);
}

TileType MapLoader::getTileType(const sf::String &name) const
{
	if (name.find("ground") != std::string::npos)
		return (GROUND);
	if (name.find("water") != std::string::npos)
		return (WATER);
	if (name.find("forest") != std::string::npos)
		return (FOREST);
	if (name.find("mountain") != std::string::npos)
		return (MOUNTAIN);
	if (name.find("road") != std::string::npos)
		return (ROAD);
	return (BUILDING);
}

bool MapLoader::loadLayer(const pugi::xml_node data, std::vector<std::vector<Tile>> &tiles, const sf::Vector2u &size) const
{
	pugi::xml_node tile;
	std::vector<std::vector<Tile>>::iterator iter;
	std::vector<Tile>::iterator iter2;
	sf::Uint32 y = 0;

	tile = data.first_child();
	iter = tiles.begin();
	iter2 = (*iter).begin();
	while (tile)
	{
		if (tile.first_attribute().as_uint() != 0 && this->_tiles.find(tile.first_attribute().as_uint()) != this->_tiles.end())
			(*iter2) = Tile(this->getTileType(this->_tiles.at(tile.first_attribute().as_uint())), this->_tiles.at(tile.first_attribute().as_uint()));
		++iter;
		if (iter == tiles.end())
		{
			iter = tiles.begin();
			++y;
		}
		iter2 = (*iter).begin() + y;
		tile = tile.next_sibling();
	}
	return (false);
}

bool MapLoader::loadBuildings(const pugi::xml_node data, std::vector<std::vector<IBuilding *>> &tiles, const sf::Vector2u &size, sf::Uint8 &qg) const
{
	pugi::xml_node tile;
	std::vector<std::vector<IBuilding *>>::iterator iter;
	std::vector<IBuilding *>::iterator iter2;
	sf::Uint32 y = 0;

	tile = data.first_child();
	iter = tiles.begin();
	iter2 = (*iter).begin();
	while (tile)
	{
		if (tile.first_attribute().as_uint() != 0 && this->_tiles.find(tile.first_attribute().as_uint()) != this->_tiles.end())
		{
			(*iter2) = new Building(this->_tiles.at(tile.first_attribute().as_uint()));
			if (this->_tiles.at(tile.first_attribute().as_uint()).find("qg") != std::string::npos)
				++qg;
		}
		++iter;
		if (iter == tiles.end())
		{
			iter = tiles.begin();
			++y;
		}
		iter2 = (*iter).begin() + y;
		tile = tile.next_sibling();
	}
	return (false);
}

bool MapLoader::loadMap(Map &map, const char *fileName)
{
	std::vector<std::vector<Tile>>::iterator iter;
	std::vector<std::vector<IUnit *>>::iterator iterUnits;
	std::vector<TileType>::iterator iter2;
	pugi::xml_parse_result result;
	pugi::xml_document doc;
	pugi::xml_node data;
	pugi::xml_node tile;

	if (!(result = doc.load_file(fileName)))
		return (printError(result));
	data = doc.child("map");
	map._tileSize = sf::Vector2u(data.attribute("tilewidth").as_uint(), data.attribute("tileheight").as_uint());
	map._size = sf::Vector2u(data.attribute("width").as_uint(), data.attribute("height").as_uint());
	map._units = std::vector<std::vector<IUnit *>>(map._size.x);
	iterUnits = map._units.begin();
	while (iterUnits != map._units.end())
	{
		(*iterUnits) = std::vector<IUnit *>(map._size.y, nullptr);
		++iterUnits;
	}
	data = doc.child("map").child("tileset").child("image");
	tile = doc.child("map").child("tileset").child("tile");
	while (tile)
	{
		this->_tiles[tile.attribute("id").as_uint() + 1] = sf::String(tile.child("properties").child("property").attribute("value").as_string());
		tile = tile.next_sibling();
	}
	data = doc.child("map").child("layer");
	while (data)
	{
		sf::String name = data.attribute("name").as_string();
		if (name == "buildings")
		{
			map._buildings = std::vector<std::vector<IBuilding *>>(map._size.x);
			std::vector<std::vector<IBuilding *>>::iterator iterBuilding = map._buildings.begin();
			while (iterBuilding != map._buildings.end())
			{
				(*iterBuilding) = std::vector<IBuilding *>(map._size.y, nullptr);
				++iterBuilding;
			}
			this->loadBuildings(data.child("data"), map._buildings, map._size, map._qg);
		}
		else
		{
			map._tiles[name] = std::vector<std::vector<Tile>>(map._size.x);
			iter = map._tiles[name].begin();
			while (iter != map._tiles[name].end())
			{
				(*iter) = std::vector<Tile>(map._size.y, Tile());
				++iter;
			}
			this->loadLayer(data.child("data"), map._tiles[name], map._size);
		}
		data = data.next_sibling();
	}
	return (true);
}
