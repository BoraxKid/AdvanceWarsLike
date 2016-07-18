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
		(*iter2) = static_cast<Tile>(tile.first_attribute().as_uint() - 1);
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

bool MapLoader::loadBuildings(const pugi::xml_node data, std::vector<std::vector<IBuilding *>> &tiles, const sf::Vector2u &size, std::map<Tile, sf::String> &tilesNames) const
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
		if (tile.first_attribute().as_uint() != 0)
		{
			std::map<Tile, sf::String>::iterator tmp = tilesNames.find(static_cast<Tile>(tile.first_attribute().as_uint() - 1));
			if (tmp != tilesNames.end())
				(*iter2) = new Building(tmp->second);
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

bool MapLoader::loadMap(Map &map, const char *fileName) const
{
	std::vector<std::vector<Tile>>::iterator iter;
	std::vector<std::vector<IUnit *>>::iterator iterUnits;
	std::vector<Tile>::iterator iter2;
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
		map._tilesNames[static_cast<Tile>(tile.attribute("id").as_uint())] = sf::String(tile.child("properties").child("property").attribute("value").as_string());
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
			this->loadBuildings(data.child("data"), map._buildings, map._size, map._tilesNames);
		}
		else
		{
			map._tiles[name] = std::vector<std::vector<Tile>>(map._size.x);
			iter = map._tiles[name].begin();
			while (iter != map._tiles[name].end())
			{
				(*iter) = std::vector<Tile>(map._size.y, GROUND);
				++iter;
			}
			this->loadLayer(data.child("data"), map._tiles[name], map._size);
		}
		data = data.next_sibling();
	}
	return (true);
}
